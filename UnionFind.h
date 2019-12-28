#pragma once

// union_find
// copy form OpenCV
// opencv/modules/imgproc/src/connectedcomponents.cpp
namespace union_find
{
    //Find the root of the tree of node i
    template<typename LabelT>
    inline static
        LabelT findRoot(const LabelT* P, LabelT i) {
        LabelT root = i;
        while (P[root] < root) {
            root = P[root];
        }
        return root;
    }

    //Make all nodes in the path of node i point to root
    template<typename LabelT>
    inline static
        void setRoot(LabelT* P, LabelT i, LabelT root) {
        while (P[i] < i) {
            LabelT j = P[i];
            P[i] = root;
            i = j;
        }
        P[i] = root;
    }

    //Find the root of the tree of the node i and compress the path in the process
    template<typename LabelT>
    inline static
        LabelT find(LabelT* P, LabelT i) {
        LabelT root = findRoot(P, i);
        setRoot(P, i, root);
        return root;
    }

    //unite the two trees containing nodes i and j and return the new root
    template<typename LabelT>
    inline static
        LabelT set_union(LabelT* P, LabelT i, LabelT j) {
        LabelT root = findRoot(P, i);
        if (i != j) {
            LabelT rootj = findRoot(P, j);
            if (root > rootj) {
                root = rootj;
            }
            setRoot(P, j, root);
        }
        setRoot(P, i, root);
        return root;
    }
}