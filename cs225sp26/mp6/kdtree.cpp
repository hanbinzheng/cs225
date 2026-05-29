/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>

using namespace std;


template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    	if (curDim >= Dim) {
	std::cout << "KDTree::smallerDimVal:" << 
		"current dimention is larger than possible dim" << std::endl;
		return false;
    	}

    	if (first[curDim] < second[curDim]) {
		return true;
    	} else if (first[curDim] == second[curDim]) {
		return (first < second); /* operator< breaks the tie */
    	} else { /* first[curDim] is larger */
		return false;
    	}
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    	/* calculate the distance square */
    	double dist_cur_sq = dist_sq(currentBest, target);
    	double dist_pot_sq = dist_sq(potential, target);

    	/* compare */
    	if (dist_cur_sq < dist_pot_sq) {
		return false;
    	} else if (dist_cur_sq == dist_pot_sq) {
		return (potential < currentBest);
    	} else { /* dist curr > dist potential */
		return true;
    	}
}

// template <int Dim>
// int KDTree<Dim>::partition(
// 	vector<Point<Dim>>& v, int idx_l, int idx_r, int idx_pivot, int dim) 
// {
//     /* pivot should within left and right */
//     if (idx_pivot < idx_l || idx_pivot > idx_r) {
// 	std::cout << "partition: pivot should be within range" << std::endl;
// 	return 114514;
//     }

//     Point<Dim> val_pivot = v[idx_pivot];
//     std::swap(v[idx_pivot], v[idx_r]); /* change the pivot to the last one */
//     int idx = idx_l; /* tmp restore of the position of pivot */
    
//     /* traverse and exchange */
//     for (int i = idx_l; i < idx_r; i++) {
//         if (smallerDimVal(v[i], val_pivot, dim)) {
//             std::swap(v[idx], v[i]);
//             idx++;
//         }
//     }
    
//     std::swap(v[idx_r], v[idx]); /* change pivot back */ 
//     return idx;
// }

// template <int Dim>
// void KDTree<Dim>::QuickSelect(
// 	vector<Point<Dim>>& v, int idx_l, int idx_r, int idx_k, int dim) 
// {
//     /* this function make sure that, 
//      * for x < idx_k, y > idx_k, v[x] <= v[idx_k] && v[idx_k] <= v[y]
//      */
//     if (idx_l > idx_r) {
// 	return;
//     }

//     int idx_pivot = (idx_l + idx_r) / 2; /* anything if fine */
//     idx_pivot = partition(v, idx_l, idx_r, idx_pivot, dim);

//     if (idx_k == idx_pivot) {
//         return;
//     } else if (idx_k < idx_pivot) {
//         QuickSelect(v, idx_l, idx_pivot - 1, idx_k, dim);
//     } else {
//         QuickSelect(v, idx_pivot + 1, idx_r, idx_k, dim);
//     }
// }

template <int Dim>
typename KDTree<Dim>::KDTreeNode *KDTree<Dim>::BuildFromVector(
	std::vector<Point<Dim>> &v, int idx_l, int idx_r, int dim)
{
	if (idx_l > idx_r || dim >= Dim) {
		return nullptr;
	}

	int idx_mid = (idx_l + idx_r) / 2;
	// QuickSelect(v, idx_l, idx_r, idx_mid, dim);
	std::nth_element(
		v.begin() + idx_l, 
		v.begin() + idx_mid, 
		v.begin() + idx_r + 1,
        	[&](const Point<Dim>& a, const Point<Dim>& b) 
		{
			return smallerDimVal(a, b, dim);
		}
    	); /* first, k, end, compare_rule */
	/* lambda notation: 
	 * [&] (functino_parameters){function definition}
	 * [] is the parameter lists, and & means it can use all local/global params
	 */
	
	KDTreeNode *node = new KDTreeNode(v[idx_mid]);

	node->left = BuildFromVector(v, idx_l, idx_mid - 1, (dim + 1) % Dim);
	node->right = BuildFromVector(v, idx_mid + 1, idx_r, (dim + 1) % Dim);

	return node;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    	/* no point case */
    	if (newPoints.empty()) {
		root = nullptr;
		size = 0;
		return;
    	}

    	/* create a new vector */
    	std::vector<Point<Dim>> points = newPoints;
    	root = BuildFromVector(points, 0, points.size() - 1, 0);
	size = points.size(); /* remeber to update size!!! */
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode *KDTree<Dim>::copy(KDTreeNode const * const subroot)
{
	if (subroot == nullptr) {
		return nullptr;
	}
	KDTreeNode *node = new KDTreeNode(subroot->point);
	node->left = copy(subroot->left);
	node->right = copy(subroot->right);
	return node;
}

template <int Dim>
void KDTree<Dim>::copy(KDTree const &other)
{
	size = other.size;
	root = copy(other.root);
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree& rhs) {
  	if (&rhs != this) {
		clear();
		copy(rhs);
  	}

  	return *this;
}

template <int Dim>
void KDTree<Dim>::clear(KDTreeNode *subroot)
{
	if (subroot == nullptr) {
		return;
	} else {
		clear(subroot->left);
		clear(subroot->right);
		delete subroot;
	}
}

template <int Dim>
void KDTree<Dim>::clear(void)
{
	clear(root);
	size = 0;
	root = nullptr;
}

template <int Dim>
void KDTree<Dim>::findNearestNeighbor(KDTreeNode *subroot, const Point<Dim> &query, 
	int dim, Point<Dim> &curr_best, double &best_dist_sq) const
{
	if (subroot == nullptr)	 {
		return;
	} else if (best_dist_sq == 0.0f) {
		return; /* perfect matching, no need to check */
	}

	KDTreeNode *near, *further;
	if(smallerDimVal(query, subroot->point, dim)) {
		near = subroot->left;
		further = subroot->right;
	} else {
		near = subroot->right;
		further = subroot->left;
	}

	findNearestNeighbor(near, query, (dim + 1) % Dim, curr_best, best_dist_sq);

	/* check whether the current is better */
	double curr_dist_sq = dist_sq(query, subroot->point);
	if (shouldReplace(curr_best, subroot->point, best_dist_sq, curr_dist_sq)) {
		curr_best = subroot->point;
		best_dist_sq = curr_dist_sq;
	}

	double dist_split = (query[dim] - subroot->point[dim]) * (query[dim] - subroot->point[dim]);
	if (best_dist_sq >= dist_split) {
		findNearestNeighbor(further, query, (dim + 1) % Dim, curr_best, best_dist_sq);
	}
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    	Point<Dim> curr_best = root->point;
	double best_dist_sq = dist_sq(root->point, query);
    	findNearestNeighbor(root, query, 0, curr_best, best_dist_sq);
    	return curr_best;
}

