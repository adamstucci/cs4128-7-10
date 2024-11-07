#include <cstdio>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

typedef long long ll;

pair<ll, ll> factorOrder(ll x, ll f) {
    ll ord = 0;
    while (x % f == 0) {
        x /= f;
        ord++;
    }

    return make_pair(ord, x);
}

// convenient modified code from lectures
// we could actually avoid storing all the prime factors and do this
// particular question in O(1) space
// pairs of (prime, order)
vector<pair<ll,ll>> primefactorize(ll x) {
    vector<pair<ll, ll>> factors;

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


ll solve(ll p, ll q) {
    if (p % q != 0) return p;

    auto primes = primefactorize(q);

    ll x = 1;
    for (auto factor : primes) {
        // want to keep dividing until just under the order i.e. when division fails
        // don't actually need the order here just need the primes themselves
        ll f = 1;
        while ((p / f) % q == 0) f *= factor.first;
        x = max(x, p / f);
    }

    return x;
}

int main() {

    int t;
    scanf("%d", &t);

    for (int i = 0; i < t; i++) {
        ll p, q;
        scanf("%lld %lld", &p, &q);

        ll x = solve(p, q);

        printf("%lld\n", x);
    }

}