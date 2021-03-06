// 埃氏筛法：返回n以内素数的个数（保存在prime[0]），复杂度O(nloglogn)
void sieve(int n) {
    memset(isprime, true, sizeof(isprime));
    isprime[0] = isprime[1] = false;
    int &p = prime[0] = 0;
    int m = (int)(sqrt((double)n)+0.5);
    for (int i=2; i<=m; ++i) {
        if (isprime[i]) {
            prime[++p] = i;
            for (int j=i*i; j<=n; j+=i) isprime[j] = false;
        }
    }
}
// 欧拉筛法：复杂度O(n)
void sieve(int n) {
    memset (isprime, true, sizeof (isprime));
    isprime[0] = isprime[1] = false;
    int &p = prime[0] = 0;
    //phi[1] = 1;
    for (int i=2; i<=n; ++i) {
        if (isprime[i]) {
            prime[++p] = i;
            //phi[i] = i - 1;
        }
        for (int j=1; i*prime[j]<=n; ++j) {
            isprime[i*prime[j]] = false;
            //当i能被prime[j]整除，那么i*prime[j+1]这个合数肯定被prime[j]乘以某个数筛掉
            if (i % prime[j] == 0) {
                //phi[i*prime[j]] = phi[i] * prime[j];
                break;
            } else {
                //phi[i*prime[j]] = phi[i] * (prime[j] - 1);
            }
        }
    }
}
/*
 *素性测试，在小范围(1e5)内判素数个数以及单个数判素数有奇效
 *不适用于大范围判素数
 */
bool is_prime(int x) {
    if (x == 2 || x == 3)   return true;
    if (x % 6 != 1 && x % 6 != 5)   return false;
    for (int i=5; i*i<=x; i+=6) {
        if (x % i == 0 || x % (i + 2) == 0) return false;
    }
    return x != 1;
}
/*
 *素性测试，对sqrt(n)内的素数进行测试即可，预处理求出sqrt(n)中的素数
 */
/*
 *素性测试，Miller_Rabin 测试算法
 *if n < 3,215,031,751, just test a = 2, 3, 5, and 7;
 *if n < 4,759,123,141, just test a = 2, 7, and 61;
 *if n < 1,122,004,669,633, just test a = 2, 13, 23, and 1662803;
 *if n < 2,152,302,898,747, just test a = 2, 3, 5, 7, and 11;
 *if n < 3,474,749,660,383, just test a = 2, 3, 5, 7, 11, and 13;
 *if n < 341,550,071,728,321, just test a = 2, 3, 5, 7, 11, 13, and 17.
 *if n < 3,825,123,056,546,413,051, just test a = 2, 3, 5, 7, 11, 13, 17, 19, and 23.
 *if n < 18,446,744,073,709,551,616 = 2^64, just test a = 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, and 37.
 */
bool test(ll x, ll n) {
    ll m = n - 1;
    while (!(m&1)) m >>= 1;
    x = pow_mod(x, m, n);
    for (; m<n-1 && x!=1 && x!=n-1; m<<=1) {
        x = mul_mod(x, x, n);  //防爆long long
    }
    return x == n-1 || (m&1) == 1;
}
bool Miller_Rabin(ll n) {
    if (n == 2 || n == 3 || n == 5 || n == 7 || n == 11
        || n == 13 || n == 17 || n == 19 || n == 23 
        || n == 29 || n == 31 || n == 37) return true;
    if (n < 2 || !(n&1)) return false;
    return test(2, n) && test(3, n) && test(5, n)
        && test(7, n) && test(11, n) && test(13, n) 
        && test(17, n) && test(19, n) && test(23, n)
        && test(29, n) && test(31, n) && test(37, n);
}

//唯一分解定理，先打个素数表优化试除法
void get_factors(int n, vector<Node> &res) {
    for (int i=1; i<=prime[0]; ++i) {
        if (n % prime[i] == 0) {
            int p = prime[i], c = 0;
            while (n % p == 0) { n /= p; c++; }
            res.push_back((Node){p, c});
        }
        if (n <= 1 || is_prime[n]) break;  //小心RE
    }
    if (n > 1) res.push_back((Node){n, 1});
}
//Pollard_rho 随机算法，找到一个n的较小的约数
ll Pollard_rho(ll n, int c) {
    ll i = 1, k = 2, x = rand()%n, y = x, d;
    while (1) {
        i++;
        x = (mul_mod(x, x, n)+c) % n;
        d = GCD(y-x, n);  //y-x可能小于0，GCD需要改动
        if (d!=1 && d!=n) return d;
        if (y == x) return n;
        if (i == k) { y = x; k <<= 1; }
    }
}
/*
 *大整数质因数分解
 *ret存储n的所有质因子，c是任意一个数字
 */
void factorize(ll n, int c, vector<ll> &ret) {
    if (n == 1) return ;
    if (Miller_Rabin(n)) {
        ret.push_back(n);
        return ;
    }
    ll p = n;
    while (p >= n) p = Pollard_rho(p, c--);
    factorize(p, c, ret);
    factorize(n/p, c, ret);
}

//约数枚举，复杂度O (sqrt(n)) By TiaoZhan
vector<int> divisor(int n)  {
    vector<int> res;
    for (int i=1; i*i<=n; ++i)  {
        if (n % i == 0) {
            res.push_back(i);
            if (n / i != i) res.push_back(n / i);
        }
    }
    return res;
}

/*
 *对正整数n，欧拉函数是少于或等于n的数中与n互质的数的数目。
 *例如euler(8)=4，因为1,3,5,7均和8互质。
 *Euler函数表达通式：euler(x)=x(1-1/p1)(1-1/p2)(1-1/p3)(1-1/p4)…(1-1/pn),
 *其中p1,p2……pn为x的所有素因数，x是不为0的整数。euler(1)=1 
 *欧拉公式的延伸：一个数的所有质因子之和是euler(n)*n/2。
 */
//单独求数 欧拉函数phi[n]
int phi(int n) {
    int ret = n, m = (int)sqrt(n+0.5);
    for (int i=1; prime[i]<=m; ++i) {
        if (n % prime[i] == 0) {
            ret = ret / prime[i] * (prime[i] - 1);
            while (n % prime[i] == 0) n /= prime[i];
        }
        if (n == 1 || is_prime[n]) break;  //小心RE
    }
    if (n > 1) ret = ret / n * (n - 1);
    return ret;
}
