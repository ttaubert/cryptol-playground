from functools import reduce

def listToInt(list):
    """Convert a byte list into a number"""
    return reduce(lambda x,y:(x<<8)+y,list)

def gf_2_128_mul(x, y):
    assert x < (1 << 128)
    assert y < (1 << 128)
    res = 0
    for i in range(127, -1, -1):
        res ^= x * ((y >> i) & 1)  # branchless
        x = (x >> 1) ^ ((x & 1) * 0xE1000000000000000000000000000000)
    assert res < 1 << 128
    return res

def multGF2(x,y):
    """Multiply two polynomials in GF(2^m)/g(w)
       g(w) = w^128 + w^7 + w^2 + w + 1
       (operands and result bits reflected)"""
    z = 0

    #while y:
    #  if y & 1:
    #    z ^= x

    #  x <<= 1
    #  y >>= 1

    while y & ((1<<128)-1):
        if y & (1<<127):
            z ^= x
        y <<= 1
        if x & 1:
            x = (x>>1)^(0xe1<<120)
        else:
            x >>= 1

    return z

def bmul(x,y):
    z = 0

    while y:
      if y & 1:
        z ^= x

      x <<= 1
      y >>= 1

    return z

def xorMultH(h,p,q):
    """Multiply (p^q) by hash key"""
    return gf_2_128_mul(h, p^q)

h = (2450725133090822402 << 64) | 1837830967247542734
x = (189151192976195584 << 64) | 13512997938921472
buf = [0, 0, 128, 176, 0, 64, 0, 16, 219, 0, 0, 177, 65, 1, 4, 132]
#print "hi: ", xorMultH(h,x,listToInt(buf)) >> 64
#print "lo: ", xorMultH(h,x,listToInt(buf)) & 0xffffffffffffffff
#print xorMultH(h,x,listToInt(buf))

#print hex(gf_2_128_mul(0x7b5b54657374566563746f725d53475d, 0x48692853686179295b477565726f6e5d))
#print hex(multGF2(0x7b5b54657374566563746f725d53475d, 0x48692853686179295b477565726f6e5d))
print hex(bmul(18432945115807726987, 18226049434058520172))
