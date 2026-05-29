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
    /**
     * @todo Implement this function!
     */
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
    /**
     * @todo Implement this function!
     */

    /* calculate the distance square */
    double dist_cur_sq = 0.0f, dist_pot_sq = 0.0f;
    for (int i = 0; i < Dim; i++) {
	dist_cur_sq += (target[i] - currentBest[i]) * (target[i] - currentBest[i]);
	dist_pot_sq += (target[i] - potential[i]) * (target[i] - potential[i]);
    } 

    /* compare */
    if (dist_cur_sq < dist_pot_sq) {
	return false;
    } else if (dist_cur_sq == dist_pot_sq) {
	return (potential < currentBest);
    } else { /* dist curr > dist potential */
	return true;
    }
}

template <int Dim>
int KDTree<Dim>::partition(
	vector<Point<Dim>>& v, int idx_l, int idx_r, int idx_pivot, int dim) 
{
    /* pivot should within left and right */
    if (idx_pivot < idx_l || idx_pivot > idx_r) {
	std::cout << "partition: pivot should be within range" << std::endl;
	return 114514;
    }

    Point<Dim> val_pivot = v[idx_pivot];
    std::swap(v[idx_pivot], v[idx_r]); /* change the pivot to the last one */
    int idx = idx_l; /* tmp restore of the position of pivot */
    
    /* traverse and exchange */
    for (int i = idx_l; i < idx_r; i++) {
        if (smallerDimVal(v[i], val_pivot, dim)) {
            std::swap(v[idx], v[i]);
            idx++;
        }
    }
    
    std::swap(v[idx_r], v[idx]); /* change pivot back */ 
    return idx;
}

template <int Dim>
void KDTree<Dim>::QuickSelect(
	vector<Point<Dim>>& v, int idx_l, int idx_r, int idx_k, int dim) 
{
    /* this function make sure that, 
     * for x < idx_k, y > idx_k, v[x] <= v[idx_k] && v[idx_k] <= v[y]
     */
    if (idx_l > idx_r) {
	return;
    }

    int idx_pivot = (idx_l + idx_r) / 2; /* anything if fine */
    idx_pivot = partition(v, idx_l, idx_r, idx_pivot, dim);

    if (idx_k == idx_pivot) {
        return;
    } else if (idx_k < idx_pivot) {
        QuickSelect(v, idx_l, idx_pivot - 1, idx_k, dim);
    } else {
        QuickSelect(v, idx_pivot + 1, idx_r, idx_k, dim);
    }
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode *KDTree<Dim>::BuildFromVector(
	std::vector<Point<Dim>> &v, int idx_l, int idx_r, int dim)
{
	if (idx_l > idx_r || dim >= Dim) {
		return nullptr;
	}

	int idx_mid = (idx_l + idx_r) / 2;
	QuickSelect(v, idx_l, idx_r, idx_mid, dim);
	// std::nth_element(
	// 	v.begin() + idx_l, 
	// 	v.begin() + idx_mid, 
	// 	v.begin() + idx_r + 1,
        // 	[&](const Point<Dim>& a, const Point<Dim>& b) 
	// 	{
	// 		return smallerDimVal(a, b, dim);
	// 	}
    	// ); /* first, k, end, compare_rule */
	// /* lambda notation: 
	//  * [&] (functino_parameters){function definition}
	//  * [] is the parameter lists, and & means it can use all local/global params
	//  */
	
	KDTreeNode *node = new KDTreeNode(v[idx_mid]);

	node->left = BuildFromVector(v, idx_l, idx_mid - 1, (dim + 1) % Dim);
	node->right = BuildFromVector(v, idx_mid + 1, idx_r, (dim + 1) % Dim);

	return node;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */

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
KDTree<Dim>::KDTree(const KDTree& other) {
  /**
   * @todo Implement this function!
   */
  copy(other);
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
  /**
   * @todo Implement this function!
   */
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
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
  clear();
}

template <int Dim>
void KDTree<Dim>::findNearestNeighbor(KDTreeNode *subroot, 
	const Point<Dim> &query, int dim, Point<Dim> &curr_best) const
{
	if (subroot == nullptr)	 {
		return;
	}

	KDTreeNode *near, *further;
	if(smallerDimVal(query, subroot->point, dim)) {
		near = subroot->left;
		further = subroot->right;
	} else {
		near = subroot->right;
		further = subroot->left;
	}

	findNearestNeighbor(near, query, (dim + 1) % Dim, curr_best);

	/* check whether this is better */
	if (shouldReplace(query, curr_best, subroot->point)) {
		curr_best = subroot->point;
	}

	double r_sq = 0;
	for (int i = 0; i < Dim; i++) {
		r_sq += (query[i] - curr_best[i]) * (query[i] - curr_best[i]);
	}
	double dist_split = (query[dim] - subroot->point[dim]) * (query[dim] - subroot->point[dim]);
	if (r_sq >= dist_split) {
		findNearestNeighbor(further, query, (dim + 1) % Dim, curr_best);
	}
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    Point<Dim> curr_best = root->point;
    findNearestNeighbor(root, query, 0, curr_best);
    return curr_best;
}

