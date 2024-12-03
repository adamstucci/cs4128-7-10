#include <cstdio>
#include <utility>
#include <vector>
#include <map>
#include <set>

// Thanks to Raveen for fixing the issue with modular arithmetic for the
// exponents of the primes

using namespace std;
typedef long long ll;

const int bound = 1000000007;

ll pow(ll x, ll n, int m) {
    if (n == 0) return 1;
    ll a = pow(x, n/2, m);
    a = a * a % m;
    if (n%2 == 1) a = a * x % m;
    return a;
}

pair<ll, ll> factorOrder(ll x, ll f) {
    ll ord = 0;
    while (x % f == 0) {
        x /= f;
        ord++;
    }

    return make_pair(ord, x);
}

// convenient modified code from lectures
// pairs of (prime, order)
vector<pair<ll,ll>> primefactorize(ll x) {
    vector<pair<ll, ll>> factors;

    // return empty list if x = 1.
    // 1 has no prime factors
    if (x == 1) return factors;

    for (ll f = 2; f*f <= x; f++) {
        if (x % f != 0) continue;
        auto info = factorOrder(x, f);
        ll o = info.first;
        factors.push_back(make_pair(f, o));
        x = info.second;
        
    }

    // in the case the originaal x was prime
    if (x != 1) factors.push_back(make_pair(x, 1));
    
    return factors;
}

inline long long modadd (long long a, long long b, long long m = bound-1) {
    return (a + b) % m;
}

inline long long modmul (long long a, long long b, long long m = bound-1) {
    return (a * b) % m;
}

// Implementation for square matrices.
struct Matrix {
    int n;
    vector<vector<long long>> v;
    
    Matrix(int _n) : n(_n) {
        v.resize(n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                v[i].push_back(0);
    }

    // need to do the multiplication and addition under modulus
    // but because they will be used to form the exponent of the prime
    // need to take modulus (bound -1) rather than modulus bound

    Matrix operator*(const Matrix &o) const {
        Matrix res(n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++)
                    res.v[i][j] = modadd(res.v[i][j], modmul(v[i][k], o.v[k][j]));
        return res;
    }

    static Matrix getIdentity(int n) {
        Matrix res(n);
        for (int i = 0; i < n; i++)
        res.v[i][i] = 1;
        return res;
    }
  
    Matrix operator^(long long k) const {
        Matrix res = Matrix::getIdentity(n);
        Matrix a = *this;
        while (k) { // building up in powers of two
            if (k&1) res = res*a;
            a = a*a;
            k /= 2;
        }
        return res;
    }
};


// we have some recurrence
// we could sove this with DP but the input is very large
// we have seen how to solve DP with large input sizes using matrix
// exponentiation, but that is for a recurrence of sums.
// Here we have a recurrence of products, but perhaps we can
// transform the problem into something over sums and apply matrix DP


// instead of solving f_x = c^{2x-6}f_{x-1}f_{x-2}f_{x-3}
// let's look at
// c^xf_x = c^{x-1}f_{x-1}c^{x-2}f_{x-2}c^{x-3}f_{x-3}
// We can think of this as some recurrence
// g_x = g_{x-1}g_{x-2}g_{x-3}

// Let h(x,p) denote the number of times the prime p is a factor of g_x
// Then h(x, p) = h(x-1, p) + h(x -2, p) + h(x - 3, p)
// This is a linear recurrence we can solve with DP
// If we can compute this for every prime factor p, we can reconstruct g_x
// and then reconstruct f_x

// f1 f2 f3 and c are all bounded by 10^9
// There can't be more than 9 distinct prime factors for such a number
// you can check this by computing the sum of the 9 smallest prime numbers
// 2*3*5*7*11*13*17*19*23 = 223092870
// 2*3*5*7*11*13*17*19*23*29 = 6469693230

// So if each number doesn't share any prime factors, then there are at
// most 36 primes we have to compute this DP for


int main() {
    ll n, f1, f2, f3, c;
    scanf("%lld %lld %lld %lld %lld", &n, &f1, &f2, &f3, &c);

    // there is an edge case when all our initial conditions are 1
    // otherwise there is at least 1 prime factor among them
    if (f1 == 1 && f2 == 1 && f3 == 1 && c == 1) {
        printf("1\n");
        return 0;
    }

    ll starts[4] = {c, f1, f2, f3};
    vector<pair<ll,ll>> factors[4];
    map<ll, ll> fmaps[4];
    set<ll> primes;

    for (int i = 0; i < 4; i++) {
        factors[i] = primefactorize(starts[i]);
        for (auto p : factors[i]) {
            fmaps[i].insert(p);
            primes.insert(p.first);
        }
    }

    // add missing primes with count 0
    for (auto p : primes) {
        for (int i = 0; i < 4; i++) {
            if (fmaps[i].count(p) == 0) fmaps[i].insert(make_pair(p, 0));
        }
    }

    map<ll, ll> h1;
    map<ll, ll> h2;
    map<ll, ll> h3;

    for (auto p: primes) {
        h1.insert(make_pair(p, fmaps[0][p] + fmaps[1][p]));
        h2.insert(make_pair(p, 2*fmaps[0][p] + fmaps[2][p]));
        h3.insert(make_pair(p, 3*fmaps[0][p] + fmaps[3][p]));
    }

    //g_1 = cf_1
    //g_2 = c^2f_2
    //g_3 = c^3f_3

    /*
    / h(n+2, p) \      / 1 1 1 \n-1  / h(3, p) \
    | h(n+1, p) |   =  | 1 0 0 |     | h(2, p) |
    \ h(n,p)    /      \ 0 1 0 /     \ h(1, p) /
    */

    // we ultimately want the h(n, p)'s

    // observe that the matrix is the same for each p, so we only need to
    // compute it once 

    Matrix m(3);
    m.v[0][0] = 1; m.v[0][1] = 1; m.v[0][2] = 1;
    m.v[1][0] = 1; m.v[1][1] = 0; m.v[1][2] = 0;
    m.v[2][0] = 0; m.v[2][1] = 1; m.v[2][2] = 0;

    m = m^(n-1);

    map<ll, ll> hn;
    for (auto p : primes) {
        // similarly to above modulus bound - 1
        hn.insert(make_pair(p, modadd(modadd(modmul(m.v[2][0],h3[p]),modmul(m.v[2][1],h2[p])),modmul(m.v[2][2],h1[p]))));
    }

    ll gn = 1;
    for (auto p : primes) {
        gn = (gn * pow(p, hn[p], bound)) % bound; //the multiplication also needs to be under modulus
    }

    // want inverse of c^n mod bound
    // this is then (c^n)^{bound-2} mod bound
    // n*(bound - 2) is too big
    // so first compute c^n mod bound -> x
    // then compute x^(bound - 2) mod bound

    ll cn = pow(c, n, bound);
    ll cnI = pow(cn, bound-2, bound);

    int res = (cnI * gn) % bound;
    printf("%d\n", res);
}