#ifndef DFS_H
#define DFS_H
#include <iterator>
#include <cmath>
#include <list>
#include <stack>

#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"

using namespace cs225;

/**
 * A depth-first ImageTraversal.
 */
class DFS : public ImageTraversal 
{
	public:
		/* constructor will automatically add start */
		DFS(const PNG & png, const Point & start, double tolerance):
			png_(png), start_(start), tolerance_(tolerance) { add(start); }

		ImageTraversal *clone() const override { return new DFS(*this); }

		unsigned png_w() const override { return png_.width(); }
		unsigned png_h() const override { return png_.height(); }
  
		ImageTraversal::Iterator begin() override;
  		ImageTraversal::Iterator end() override;

  		void add(const Point & point) override;
  		Point pop() override;
  		Point peek() const override;
  		bool empty() const override;

		double getDelta(Point const &p) const override
		{
			if (!point_in_png(p, png_)) {
				std::cout << "[BFS::getDelta]: point out of image" << std::endl;
				return 99999.0f;
			}
			HSLAPixel *p1 = png_.getPixel(p.x, p.y);
			HSLAPixel *p2 = png_.getPixel(start_.x, start_.y);

			return calculateDelta(*p1, *p2);
		}
		bool in_tolerance(Point const &p) const override { return (getDelta(p) < tolerance_); }

	private:
		PNG const png_;
		Point start_;
		double tolerance_;
		std::stack<Point> stk_;

		static inline bool point_in_png(Point const &point, PNG const &png)
		{
			unsigned w = png.width();
			unsigned h = png.height();
			return ((point.x <= w - 1) && (point.y <= h -1));
		}
};

#endif
