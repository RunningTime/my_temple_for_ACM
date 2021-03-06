//加法取模
void add_mod(int &a, int b, int mod) {
    a += b;
    if (a >= mod) {
        a -= mod;
    }
}
//乘法取模，a * b % mod，复杂度O(logb)
ll mul_mod(ll a, ll b, ll mod) {
    ll ret = 0;
    a = (a % mod + mod) % mod;
    b = (b % mod + mod) % mod;
    for (; b; b>>=1) {
        if (b & 1) {
            ret += a;
            if (ret >= mod) ret -= mod;
        }
        a <<= 1;
        if (a >= mod) a -= mod;
    }
    return ret;
}
//乘法取模，复杂度O(1) 
ll mul_mod(ll a, ll b, ll mod) {
    ll ret = a*b - (ll)((double)a*b/mod+0.001)*mod;
    return ret < 0 ? ret + mod : ret;
}
//乘方取模(快速幂)，x ^ n % mod，复杂度 O(logn)
ll pow_mod(ll x, ll n, ll mod) {
    ll ret = 1;
    for (; n; n>>=1) {
        if (n & 1) ret = ret * x % mod;
        x = x * x % mod;
    }
    return ret;
}
//离散对数，a^x=b(%mod)，大步小步算法，复杂度 O(mod^(1/2)log(mod))
int log_mod(int a, int b, int mod) {
    int m, v, e = 1;
    m = (int) sqrt (mod + 0.5);
    v = Inv (pow_mod (a, m));
    std::map<int, int> ID;
    ID[1] = 0;
    for (int i=1; i<m; ++i) {
        e = (ll) e * a % mod;
        if (!ID.count (e)) {
            ID[e] = i;
        }
    }
    for (int i=0; i<m; ++i) {
        if (ID.count (b)) {
            return i * m + ID[b];
        }
        b = (ll) b * v % mod;
    }
    return -1;
}
