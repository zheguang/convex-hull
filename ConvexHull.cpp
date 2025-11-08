#include <bits/stdc++.h>

using namespace std;

using Point = pair<int, int>;
using Points = vector<Point>;
using Hull = pair<Points, Points>; /// lower hull, upper hull

void print(const Point & p)
{
    cout << "(" << p.first << ", " << p.second << ")" << endl;
}

void print(const Points & ps)
{
    cout << endl;
    for (const auto & p : ps)
        print(p);
    cout << endl;
}

void print(const Hull & hull)
{
    cout << "## lower hull: ";
    print(hull.first);
    cout << "## upper hull: ";
    print(hull.second);
}

// Alphabetical sort
Points asort(const Points & ps)
{
    if (ps.size() <= 1)
        return Points(ps);

    Points left = asort(Points(ps.begin(), ps.begin() + ps.size() / 2));
    Points right = asort(Points(ps.begin() + ps.size() / 2, ps.end()));

    Points res;
    res.reserve(ps.size());
    auto l = left.begin();
    auto r = right.begin();
    while (l != left.end() && r != right.end())
        res.push_back(*l < *r ? *l++ : *r++);

    res.insert(res.end(), l, left.end());
    res.insert(res.end(), r, right.end());

    return res;
}

/// Vector from point o to p
Point v(const Point & o, const Point & p)
{
    return Point { p.first - o.first, p.second - o.second };
}

int turn(const Point & o, const Point & a, const Point & b)
{
    Point oa = v(o, a);
    Point ob = v(o, b); 
    return oa.first * ob.second - oa.second * ob.first;
}

Hull andrew(const Points & ps_unsorted)
{
    /// Sort the points alphabetically in a sequence
    /// Put first two points in the sequence into the hull
    /// Consider the turn formed by the last two points in the hull and the next point X in the sequence
    /// If right turn, then pop the last point on the hull, and reconsider.
    /// If left turn, then append X to the hull, and move on to the next in the sequence
    /// If no turn, then replace the last point with X, and move on

    Points ps = asort(ps_unsorted);

    if (ps.size() <= 2)
        return Hull { ps, Points(ps.rbegin(), ps.rend()) };

    Hull hull;
    {
        auto pi = ps.begin();
        Points res;
        while (pi != ps.end())
        {
            /// Minimum hull
            if (res.size() < 2)
            {
                res.push_back(*pi++);
                continue;
            }

            int t = turn(res[res.size() - 2], res.back(), *pi);
            /// Right turn
            if (t < 0)
                res.pop_back();
            /// No turn
            else if (t == 0)
                res.back() = *pi++;
            /// Left turn
            else
                res.push_back(*pi++);
        }

        hull.first = res;
    }

    {
        auto pi = ps.rbegin();
        Points res;
        while (pi != ps.rend())
        {
            /// Minimum hull
            if (res.size() < 2)
            {
                res.push_back(*pi++);
                continue;
            }

            int t = turn(res[res.size() - 2], res.back(), *pi);
            /// Right turn
            if (t < 0)
                res.pop_back();
            /// No turn
            else if (t == 0)
                res.back() = *pi++;
            /// Left turn
            else
                res.push_back(*pi++);
        }
        hull.second = res;
    }

    return hull;
}

/// Tests
void testOutput(const string & test, const Points & input)
{
    cout << endl << "# " << test << endl;
    print(andrew(input));
    cout << "---" << endl;
}

void testCases()
{
    /// 1. Empty or minimal sets
    testOutput("Empty set", {});
    testOutput("Single point", {{5, 5}});
    testOutput("Two points", {{1, 1}, {3, 3}});
    testOutput("Triangle (all on hull)", {{0, 0}, {2, 0}, {1, 2}});

    /// 2. Collinear points
    testOutput("Horizontal", {{1, 1}, {2, 1}, {3, 1}, {4, 1}});
    testOutput("Vertical", {{2, 1}, {2, 2}, {2, 3}, {2, 4}});
    testOutput("Diagonal", {{1, 1}, {2, 2}, {3, 3}, {4, 4}});

    /// 3. Square/Rectangle:
    testOutput("Square", {{0, 0}, {0, 4}, {4, 4}, {4, 0}});
    testOutput("Rectangle", {{0, 0}, {0, 3}, {5, 3}, {5, 0}});

    /// 4. Points with interior (some inside hull):
    testOutput("With interior", {
        {0, 0}, {4, 0}, {4, 4}, {0, 4},  /// Outer square
        {2, 2}, {1, 1}, {3, 3}           /// Interior points
    });

    /// 5. Convex polygon (all points on hull):
    testOutput("Hexagon", {{2, 0}, {4, 1}, {4, 3}, {2, 4}, {0, 3}, {0, 1}});

    /// 6. Duplicate points:
    testOutput("duplicates", {{1, 1}, {1, 1}, {3, 1}, {3, 3}, {1, 3}});

    /// 7. Many collinear points on hull edge:
    testOutput("edge_points", {
        {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0},  /// Bottom edge
        {4, 4}, {0, 4}                           /// Top corners
    });
    /// 8. Star pattern:
    testOutput("star", {
        {5, 0}, {6, 3}, {9, 3}, {7, 5}, {8, 8},  /// Outer points
        {5, 6}, {2, 8}, {3, 5}, {1, 3}, {4, 3}   /// Inner points
    });

    /// 9. Pre-sorted points:
    testOutput("sorted", {{0, 0}, {1, 1}, {2, 0}, {3, 1}, {4, 0}});
    /// 10. Reverse sorted:
    testOutput("reverse", {{5, 5}, {4, 4}, {3, 3}, {2, 2}, {1, 1}, {0, 0}});

    /// 11. Shapes - all points on hull
    testOutput("diamond", {{2, 0}, {4, 2}, {2, 4}, {0, 2}});
    testOutput("l_shape", {{0, 0}, {3, 0}, {3, 1}, {1, 1}, {1, 3}, {0, 3}});

    /// 12. Large coordinate values
    testOutput("large", {{-1000, -1000}, {1000, -1000}, {1000, 1000}, {-1000, 1000}, {0, 0}});
}


int main()
{
    testCases();
    return 0;
}
