#include <cstdio>
#include <utility>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef pair<ll, ll> pt;

// be careful not to use x and y for anything else if doing this
#define x first
#define y second

pt operator-(pt a, pt b) {
  return pt(a.x - b.x, a.y - b.y);
}

double cross(pt a, pt b) {
    return a.x*b.y - a.y*b.x;
}

// true if left or straight
// sometimes useful to instead return an int
// -1, 0 or 1: the sign of the cross product
bool ccw(pt a, pt b, pt c) {
    return cross(b - a, c - a) >= 0;
}

vector<pt> half_hull(vector<pt> pts) {
  vector<pt> res;
  for (int i = 0; i < pts.size(); i++) {
    // ccw means we have a left turn; we don't want that
    while (res.size() >= 2 &&
           ccw(pts[i], res[res.size()-1], res[res.size()-2])) {
      res.pop_back();
    }
    res.push_back(pts[i]);
  }
  return res;
}

bool seen[2000010];

int main() {
    vector<pt> linear_pts;
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        ll a, b; //x and y
        scanf("%lld %lld", &a, &b);

        // y is quadradic in x because we can fit a porabala that intersects this point
        // hence y - x^2 is linear in x and we can fit a line through it 

        // transform the points from a quadradic relationship to a linear one

        linear_pts.push_back(make_pair(a, b - a*a));
    }

    // we now want to count the number of segments in the top part of the
    // convex hull of these linear poiints 

    // applying monotone chain algorithm we can compute the top hull.
    // for the bottom hull we'd reverse the order and compute again, giving us
    // the full hull, but we only need the top

    // sorts the points in lexicographical order
    sort(linear_pts.begin(), linear_pts.end());
    reverse(linear_pts.begin(), linear_pts.end());

    auto top = half_hull(linear_pts);

    // count points that don't share an x coordinate
    int count = 0;
    for (auto p : top) {
        if (seen[p.x + 1000000]) continue;
        count++;
        seen[p.x + 1000000] = true;
    }

    printf("%d\n", count -1);

}