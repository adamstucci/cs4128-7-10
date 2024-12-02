#include <cstdio>
#include <utility>
#include <cmath>

using namespace std;

typedef pair<int, int> pt;

// be careful not to use x and y for anything else if doing this
#define x first
#define y second

int cross(pt a, pt b) {
    return a.x*b.y-a.y*b.x;
}

// easiest approach in my opinion is via area

// if point is interior, it will form 3 disjoint triangles inside the triangle.
// Their area will sum to the area of the triangle

// if point is exterior, we get 3 triangles which overlap and sum of area
// will be greater than the triangle

// use cross product to compute area
// gives us signed area of parallalegram..just take absolute
// no real need to divide by 2

// the fact that we are testing the origin rather than an arbitrary point makes
// things a bit more convenient but the general case essentially
// just involves geometrically translating the problem to the origin

// working with integers so everything should be exact...no precision error

int main() {
    
    // I'm just going to read the input from stdin
    int a1,a2,b1,b2,c1,c2;
    while (scanf("%d,%d,%d,%d,%d,%d", &a1, &a2, &b1, &b2, &c1, &c2) == 6) {
        
        // create pairs so a little easier to use
        pt A = make_pair(a1, a2);
        pt B = make_pair(b1, b2);
        pt C = make_pair(c1, c2);

        // Anchor the large triangle at point A
        
        // compute direction vector A to B
        pt AB = make_pair(B.x - A.x, B.y - A.y);

        // compute direction vector A to C
        pt AC = make_pair(C.x - A.x, C.y - A.y);

        int total_area = abs(cross(AB, AC));

        // because forming with origin, already is a direction vector
        int ABZ_area = abs(cross(A, B));
        int ACZ_area = abs(cross(A, C));
        int BCZ_area = abs(cross(B, C));

        if (total_area == ABZ_area + ACZ_area + BCZ_area) printf("YES\n");
        else printf("NO\n");

    }
}