#include <stdio.h>

#include "lib/mpi.h"
#include "lib/mpi-priv.h"

// TODO https://github.com/GaloisInc/saw-script/issues/120
#define ARGCHK(X,Y) /* {if(!(X)){return (Y);}} */

#define s_mp_alloc(nb, ni) calloc(nb, ni)
#define s_mp_free(ptr) {if(ptr) free(ptr);}
#define s_mp_copy(sp, dp, count) memcpy(dp, sp, (count) * sizeof(mp_digit))

// TODO SAW doesn't support memset with a symbolic length.
//#define s_mp_setz(dp, count) memset(dp, 0, (count) * sizeof(mp_digit))
const mp_digit zeros[MP_DEFPREC] = { 0 };
#undef s_mp_setz
#define s_mp_setz(dp, count) s_mp_copy(zeros, dp, count)

// TODO We can't verify macros.
#undef MP_MACRO
#define MP_MACRO 0
#undef s_mp_clamp

/*
  mp_cmp_z(a)

  Compare a <=> 0.  Returns <0 if a<0, 0 if a=0, >0 if a>0.
 */

int mp_cmp_z(const mp_int *a)
{
  if(MP_SIGN(a) == MP_NEG)
    return MP_LT;
  else if(MP_USED(a) == 1 && MP_DIGIT(a, 0) == 0)
    return MP_EQ;
  else
    return MP_GT;

} /* end mp_cmp_z() */

/* {{{ mp_cmp_d(a, d) */

/*
  mp_cmp_d(a, d)

  Compare a <=> d.  Returns <0 if a<d, 0 if a=d, >0 if a>d
 */

int mp_cmp_d(const mp_int *a, mp_digit d)
{
  ARGCHK(a != NULL, MP_EQ);

  if(MP_SIGN(a) == MP_NEG)
    return MP_LT;

  return s_mp_cmp_d(a, d);

} /* end mp_cmp_d() */

/* {{{ s_mp_cmp_d(a, d) */

/* Compare |a| <=> d, return 0 if equal, <0 if a<d, >0 if a>d             */
int s_mp_cmp_d(const mp_int *a, mp_digit d)
{
  if(MP_USED(a) > 1)
    return MP_GT;

  if(MP_DIGIT(a, 0) < d)
    return MP_LT;
  else if(MP_DIGIT(a, 0) > d)
    return MP_GT;
  else
    return MP_EQ;

} /* end s_mp_cmp_d() */

/* {{{ mp_cmp(a, b) */

int mp_cmp(const mp_int *a, const mp_int *b)
{
  ARGCHK(a != NULL && b != NULL, MP_EQ);

  if(MP_SIGN(a) == MP_SIGN(b)) {
    int  mag;

    if((mag = s_mp_cmp(a, b)) == MP_EQ)
      return MP_EQ;

    if(MP_SIGN(a) == MP_ZPOS)
      return mag;
    else
      return -mag;

  } else if(MP_SIGN(a) == MP_ZPOS) {
    return MP_GT;
  } else {
    return MP_LT;
  }

} /* end mp_cmp() */

/* Compare |a| <=> |b|, return 0 if equal, <0 if a<b, >0 if a>b           */
int s_mp_cmp(const mp_int *a, const mp_int *b)
{
  mp_size used_a = MP_USED(a);
  {
    mp_size used_b = MP_USED(b);

    if (used_a > used_b)
      goto IS_GT;
    if (used_a < used_b)
      goto IS_LT;
  }
  {
    mp_digit *pa, *pb;
    mp_digit da = 0, db = 0;

#define CMP_AB(n) if ((da = pa[n]) != (db = pb[n])) goto done

    pa = MP_DIGITS(a) + used_a;
    pb = MP_DIGITS(b) + used_a;
    while (used_a >= 4) {
      pa     -= 4;
      pb     -= 4;
      used_a -= 4;
      CMP_AB(3);
      CMP_AB(2);
      CMP_AB(1);
      CMP_AB(0);
    }
    while (used_a-- > 0 && ((da = *--pa) == (db = *--pb)))
      /* do nothing */;
done:
    if (da > db)
      goto IS_GT;
    if (da < db) 
      goto IS_LT;
  }
  return MP_EQ;
IS_LT:
  return MP_LT;
IS_GT:
  return MP_GT;
} /* end s_mp_cmp() */

/* }}} */

/* {{{ mp_cmp_mag(a, b) */

/*
  mp_cmp_mag(a, b)

  Compares |a| <=> |b|, and returns an appropriate comparison result
 */

int mp_cmp_mag(mp_int *a, mp_int *b)
{
  ARGCHK(a != NULL && b != NULL, MP_EQ);

  return s_mp_cmp(a, b);
} /* end mp_cmp_mag() */

/* {{{ mp_isodd(a) */

/*
  mp_isodd(a)

  Returns a true (non-zero) value if a is odd, false (zero) otherwise.
 */
int mp_isodd(const mp_int *a)
{
  ARGCHK(a != NULL, 0);

  return (int)(MP_DIGIT(a, 0) & 1);

} /* end mp_isodd() */

/* }}} */

/* {{{ mp_iseven(a) */

int mp_iseven(const mp_int *a)
{
  return !mp_isodd(a);

} /* end mp_iseven() */

/* {{{ mp_copy(from, to) */

/*
  mp_copy(from, to)

  Copies the mp_int 'from' to the mp_int 'to'.  It is presumed that
  'to' has already been initialized (if not, use mp_init_copy()
  instead). If 'from' and 'to' are identical, nothing happens.
 */

mp_err mp_copy(const mp_int *from, mp_int *to)
{
  ARGCHK(from != NULL && to != NULL, MP_BADARG);

  // TODO https://github.com/GaloisInc/saw-script/issues/120
  //if(from == to)
    //return MP_OKAY;

  { /* copy */
    //mp_digit   *tmp;

    /*
      If the allocated buffer in 'to' already has enough space to hold
      all the used digits of 'from', we'll re-use it to avoid hitting
      the memory allocater more than necessary; otherwise, we'd have
      to grow anyway, so we just allocate a hunk and make the copy as
      usual
     */
    if(MP_ALLOC(to) >= MP_USED(from)) {
      s_mp_setz(MP_DIGITS(to) + MP_USED(from), MP_ALLOC(to) - MP_USED(from));
      s_mp_copy(MP_DIGITS(from), MP_DIGITS(to), MP_USED(from));
    } else {
      // TODO Not supported by SAW.
      return MP_MEM;
      /*if((tmp = s_mp_alloc(MP_ALLOC(from), sizeof(mp_digit))) == NULL)
        return MP_MEM;

      s_mp_copy(MP_DIGITS(from), tmp, MP_USED(from));

      if(MP_DIGITS(to) != NULL) {
#if MP_CRYPTO
        s_mp_setz(MP_DIGITS(to), MP_ALLOC(to));
#endif
        s_mp_free(MP_DIGITS(to));
      }

      MP_DIGITS(to) = tmp;
      MP_ALLOC(to) = MP_ALLOC(from);*/
    }

    /* Copy the precision and sign from the original */
    MP_USED(to) = MP_USED(from);
    MP_SIGN(to) = MP_SIGN(from);
  } /* end copy */

  return MP_OKAY;

} /* end mp_copy() */

/* }}} */

/* {{{ mp_abs(a, b) */

/*
  mp_abs(a, b)

  Compute b = |a|.  'a' and 'b' may be identical.
 */

mp_err mp_abs(const mp_int *a, mp_int *b)
{
  mp_err   res;

  ARGCHK(a != NULL && b != NULL, MP_BADARG);

  if((res = mp_copy(a, b)) != MP_OKAY)
    return res;

  MP_SIGN(b) = MP_ZPOS;

  return MP_OKAY;

} /* end mp_abs() */

/* }}} */

/* {{{ mp_neg(a, b) */

/*
  mp_neg(a, b)

  Compute b = -a.  'a' and 'b' may be identical.
 */

mp_err mp_neg(const mp_int *a, mp_int *b)
{
  mp_err   res;

  ARGCHK(a != NULL && b != NULL, MP_BADARG);

  if((res = mp_copy(a, b)) != MP_OKAY)
    return res;

  if(s_mp_cmp_d(b, 0) == MP_EQ)
    MP_SIGN(b) = MP_ZPOS;
  else
    MP_SIGN(b) = (MP_SIGN(b) == MP_NEG) ? MP_ZPOS : MP_NEG;

  return MP_OKAY;

} /* end mp_neg() */

/* }}} */

/* {{{ mp_zero(mp) */

/*
  mp_zero(mp)

  Set mp to zero.  Does not change the allocated size of the structure,
  and therefore cannot fail (except on a bad argument, which we ignore)
 */
void mp_zero(mp_int *mp)
{
  // TODO https://github.com/GaloisInc/saw-script/issues/120
  /*if(mp == NULL)
    return;*/

  s_mp_setz(MP_DIGITS(mp), MP_ALLOC(mp));
  MP_USED(mp) = 1;
  MP_SIGN(mp) = MP_ZPOS;
} /* end mp_zero() */

/* }}} */

/* {{{ s_mp_clamp(mp) */

/* Remove leading zeroes from the given value                             */
void s_mp_clamp(mp_int *mp)
{
  mp_size used = MP_USED(mp);
  while (used > 1 && MP_DIGIT(mp, used - 1) == 0)
    --used;
  MP_USED(mp) = used;
} /* end s_mp_clamp() */

/* }}} */

/* {{{ s_mp_exch(a, b) */

/* Exchange the data for a and b; (b, a) = (a, b)                         */
void s_mp_exch(mp_int *a, mp_int *b)
{
  // TODO SAW doesn't support swapping digits pointers.
  /*mp_int   tmp;

  tmp = *a;
  *a = *b;
  *b = tmp;*/

  if (MP_ALLOC(a) < MP_USED(b) || MP_ALLOC(b) < MP_USED(a)) {
    return;
  }

  mp_digit tmp_digits[MP_DEFPREC];
  s_mp_copy(MP_DIGITS(b), tmp_digits, MP_USED(b));
  s_mp_copy(MP_DIGITS(a), MP_DIGITS(b), MP_USED(a));
  s_mp_copy(tmp_digits, MP_DIGITS(a), MP_USED(b));

  mp_size tmp_used = MP_USED(b);
  MP_USED(b) = MP_USED(a);
  MP_USED(a) = tmp_used;

  mp_sign tmp_sign = MP_SIGN(b);
  MP_SIGN(b) = MP_SIGN(a);
  MP_SIGN(a) = tmp_sign;
} /* end s_mp_exch() */

/* }}} */

/* {{{ s_mp_rshd(mp, p) */

/*
   Shift mp rightward by p digits.  Maintains the invariant that
   digits above the precision are all zero.  Digits shifted off the
   end are lost.  Cannot fail.
 */
void s_mp_rshd(mp_int *mp, mp_size p)
{
  mp_size  ix;
  mp_digit *src, *dst;

  if(p == 0)
    return;

  /* Shortcut when all digits are to be shifted off */
  if(p >= MP_USED(mp)) {
    s_mp_setz(MP_DIGITS(mp), MP_ALLOC(mp));
    MP_USED(mp) = 1;
    MP_SIGN(mp) = MP_ZPOS;
    return;
  }

  /* Shift all the significant figures over as needed */
  dst = MP_DIGITS(mp);
  src = dst + p;
  for (ix = MP_USED(mp) - p; ix > 0; ix--)
    *dst++ = *src++;

  MP_USED(mp) -= p;
  /* Fill the top digits with zeroes */
  /*while (p-- > 0) {
    *dst++ = 0;
  }*/
  s_mp_setz(MP_DIGITS(mp) + MP_USED(mp), MP_ALLOC(mp) - MP_USED(mp));

} /* end s_mp_rshd() */

/* }}} */

/* {{{ s_mp_pad(mp, min) */

/* Make sure the used size of mp is at least 'min', growing if needed     */
mp_err s_mp_pad(mp_int *mp, mp_size min)
{
  if(min > MP_USED(mp)) {
    //mp_err  res;

    /* Make sure there is room to increase precision  */
    if (min > MP_ALLOC(mp)) {
      // TODO SAW doesn't support allocation.
      /*if ((res = s_mp_grow(mp, min)) != MP_OKAY)
        return res;*/
      return MP_MEM;
    } else {
      s_mp_setz(MP_DIGITS(mp) + MP_USED(mp), min - MP_USED(mp));
    }

    /* Increase precision; should already be 0-filled */
    MP_USED(mp) = min;
  }

  return MP_OKAY;

} /* end s_mp_pad() */

/* }}} */

/* {{{ s_mp_lshd(mp, p) */

/*
   Shift mp leftward by p digits, growing if needed, and zero-filling
   the in-shifted digits at the right end.  This is a convenient
   alternative to multiplication by powers of the radix
 */
mp_err s_mp_lshd(mp_int *mp, mp_size p)
{
  mp_err res;
  unsigned int ix;

  if (p == 0 /*|| p > MP_USED(mp) + p*/) // TODO
    return MP_OKAY;

  if (MP_USED(mp) == 1 && MP_DIGIT(mp, 0) == 0)
    return MP_OKAY;

  if((res = s_mp_pad(mp, MP_USED(mp) + p)) != MP_OKAY)
    return res;

  /* Shift all the significant figures over as needed */
  for (ix = MP_USED(mp) - p; ix-- > 0;) {
    MP_DIGIT(mp, ix + p) = MP_DIGIT(mp, ix);
  }

  /* Fill the bottom digits with zeroes */
  /*for(ix = 0; (mp_size)ix < p; ix++)
    MP_DIGIT(mp, ix) = 0;*/

  return MP_OKAY;

} /* end s_mp_lshd() */

/* }}} */
