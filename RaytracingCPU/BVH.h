#pragma once

#include "common.h"
#include "hittable.h"
#include <stack>
#include <assert.h>

//ingo wald 2007
class BVH : public HittableList {
public:
    BVH() : m_IsBuilt(false), m_RootNode(nullptr) {}
    BVH(std::initializer_list<il_unique_ptr_wrapper<Hittable>> objs) : HittableList(std::move(objs)), m_IsBuilt(false), m_RootNode(nullptr) {
        build();
    }

    struct Node : public Hittable {
        Node() : left(nullptr), right(nullptr) {}

        size_t objsBegin;
        size_t objsEnd;

        Hittable* left;
        Hittable* right;

        AABB bb;

        virtual bool boundingBox(AABB& aabb) const {
            aabb = bb;
            return true;
        }

        virtual bool hit(const Ray& ray, double tMin, double tMax, HitInfo& hitInfo) const {
            return left->hit(ray, tMin, hitInfo.t, hitInfo)&& right->hit(ray, tMin, hitInfo.t, hitInfo);
        }
    };

    struct Bin {
        Bin() : leftArea(0.0), leftCount(0), count(0) {}
        double leftArea;
        int leftCount;
        int count;

        AABB bb;
        AABB cbb;
    };

    void build() {
        AABB centersBB;
        buildCentersBoundingBox(centersBB, 0, m_Objects.size());

        m_RootNode = allocNode();
        m_RootNode->objsBegin = 0;
        m_RootNode->objsEnd = m_Objects.size();
        buildNode(m_RootNode, centersBB);

        m_IsBuilt = true;
        
    }


#define NODE(hittable) ((Node*)(hittable))
#define BINS_COUNT 8
    void buildNode(Node* node, const AABB& cbb) {
        if (node->objsEnd - node->objsBegin <= 1) {
            return;
        }


        vec3 extent = cbb.extent();
        assert(extent[0] >= 0 && extent[1] >= 0 && extent[2] >= 0);

        int axisIndex = 0;
        for (int i = axisIndex + 1; i < 3; i++) {
            if ((extent[i]) > (extent[axisIndex]))
                axisIndex = i;
        }

        double binsLength = extent[axisIndex];

        Bin bins[BINS_COUNT];
        for (size_t i = node->objsBegin; i < node->objsEnd; i++) {
            AABB objBB;
            if (m_Objects[i]->boundingBox(objBB)) {
                int binIndex = calcBinIndex(objBB.center(), cbb, binsLength, axisIndex);

                bins[binIndex].bb.extend(objBB);
                bins[binIndex].cbb.extend(objBB.center());
                bins[binIndex].count++;
            }
            else
                assert(false);
        }

        //extend all bins bbs by small amount (???)
        //

        int leftCount = 0;
        AABB temp;
        for (size_t i = 1; i < BINS_COUNT; i++) {
            temp.extend(bins[i - 1].bb);
            bins[i].leftArea = temp.area();
            leftCount += bins[i - 1].count;
            bins[i].leftCount = leftCount;
        }

        int splitIndex = BINS_COUNT;
        double cost = std::numeric_limits<double>::max();
        int rightCount = 0;
        temp = AABB();
        for (int i = BINS_COUNT - 1; i >= 1; i--) {
            rightCount += bins[i].count;
            temp.extend(bins[i].bb);

            double tempCost = rightCount * temp.area() + bins[i].leftArea * bins[i].leftCount;
            if (tempCost < cost) {
                cost = tempCost;
                splitIndex = i;
            }
        }


        //split

        int i = node->objsBegin, 
            j = node->objsEnd - 1;
        for (i; i < j ; i++) {
            AABB objBB;
            if (m_Objects[i]->boundingBox(objBB)) {
                int binIndex = calcBinIndex(objBB.center(), cbb, binsLength, axisIndex);

                if (binIndex >= splitIndex) {
                    AABB obj2BB;

                    while (m_Objects[j]->boundingBox(obj2BB) && calcBinIndex(objBB.center(), cbb, binsLength, axisIndex) >= splitIndex && i < j) {
                        j--;
                    }

                    if (i == j) {
                        break;
                    }
                    std::swap(m_Objects[i], m_Objects[j]);
                }
            }
            else
                assert(false);
        }

        if (i > node->objsBegin) {
            Hittable* childNode = nullptr;
            if (i - node->objsBegin > 1) {

                childNode = allocNode();
                NODE(childNode)->objsBegin = node->objsBegin;
                NODE(childNode)->objsEnd = i;

                AABB cbb;
                AABB bb;
                for (int i = 0; i < splitIndex; i++) {
                    cbb.extend(bins[i].cbb);
                    bb.extend(bins[i].bb);
                }

                NODE(childNode)->bb = bb;

                buildNode(NODE(childNode), cbb);
            }
            else {
                childNode = m_Objects[node->objsBegin].get();
            }


            node->left = childNode;
        }

        if (j < node->objsEnd - 1) {
            Hittable* childNode = nullptr;

            if (node->objsEnd - j > 1) {
                childNode = allocNode();
                NODE(childNode)->objsBegin = j;
                NODE(childNode)->objsEnd = node->objsEnd;

                AABB cbb;
                AABB bb;
                for (int i = splitIndex; i < BINS_COUNT; i++) {
                    cbb.extend(bins[i].cbb);
                    bb.extend(bins[i].bb);
                }

                NODE(childNode)->bb = bb;

                buildNode(NODE(childNode), cbb);
            }
            else {
                childNode = m_Objects[j].get();
            }


            node->right = childNode;
        }
    }

    virtual bool hit(const Ray& ray, double tMin, double tMax, HitInfo& hitInfo) const override {
        hitInfo.t = tMax;

        return m_RootNode->hit(ray, tMin, tMax, hitInfo);
    }
private:
    bool m_IsBuilt;

    Node* m_RootNode;





    inline int calcBinIndex(const vec3& center, const AABB& cbb, double extentLength, int axis) {
        constexpr double k = 0.999; //?????
        return k*BINS_COUNT*(center[axis] - cbb.m_Min[axis]) / extentLength;
    }

    inline int calcObjBinIndex(shared_ptr<Hittable> obj, const AABB& cbb, double extentLength, int axis) {
        AABB objBB;
        if (obj->boundingBox(objBB)) {
            int binIndex = calcBinIndex(objBB.center(), cbb, extentLength, axis);

            return binIndex;
        }

        assert(false);
        return -1;
    }

    Node* allocNode() {
        return new Node();
    }

    void buildCentersBoundingBox(AABB& bb, size_t begin, size_t end) {
        bb = AABB();

        for (size_t i = begin; i < end; i++)
        {
            auto& obj = m_Objects[i];

            AABB objBB;
            if (obj->boundingBox(objBB))
                bb.extend(bb.center());
        }
    }
};