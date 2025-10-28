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
        for (int i = 0; i < 2; i++)
            res.push_back(*pi++);

        while (pi != ps.end())
        {
            int t = turn(res[res.size() - 2], res.back(), *pi);
            /// Right turn
            if (t < 0)
            {
                /// Minimum hull
                if (ps.size() < 2)
                    res.push_back(*pi++);
                else
                    res.pop_back();
            }
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
        for (int i = 0; i < 2; i++)
            res.push_back(*pi++);

        while (pi != ps.rend())
        {
            int t = turn(res[res.size() - 2], res.back(), *pi);
            /// Right turn
            if (t < 0)
            {
                /// Minimum hull
                if (ps.size() < 2)
                    res.push_back(*pi++);
                else
                    res.pop_back();
            }
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

int main()
{
    Points ps { {1, 2}, {2, 2}, {3, 2}, {2, 3}, {2, 1} };
    print(ps);
    
    Points ps2 = asort(ps);
    print(ps2);

    return 0;
}
