#pragma once
#include "YunutyEngine.h"

class QuadTreeDebugger : public GameObjectPool<D2DRectangle>
{
private:
    void Borrow(const Collider2D::QuadTreeNode* node) {
        auto leftVertical = GameObjectPool<D2DRectangle>::Borrow();
        auto rightVertical = GameObjectPool<D2DRectangle>::Borrow();
        auto topHorizontal = GameObjectPool<D2DRectangle>::Borrow();
        auto bottomHorizontal = GameObjectPool<D2DRectangle>::Borrow();
        auto center = Vector2d((node->xInterval.left + node->xInterval.right) / 2, (node->yInterval.left + node->yInterval.right) / 2);
        leftVertical->GetTransform()->SetWorldPosition(Vector2d(node->xInterval.left, center.y));
        rightVertical->GetTransform()->SetWorldPosition(Vector2d(node->xInterval.right, center.y));
        topHorizontal->GetTransform()->SetWorldPosition(Vector2d(center.x, node->yInterval.right));
        bottomHorizontal->GetTransform()->SetWorldPosition(Vector2d(center.x, node->yInterval.left));

        leftVertical->width = 2;
        rightVertical->width = 2;
        topHorizontal->height = 2;
        bottomHorizontal->height = 2;

        leftVertical->height = node->yInterval.GetLength();
        rightVertical->height = node->yInterval.GetLength();
        topHorizontal->width = node->xInterval.GetLength();
        bottomHorizontal->width = node->xInterval.GetLength();
        leftVertical->color = D2D1::ColorF::Aqua;
        rightVertical->color = D2D1::ColorF::Aqua;
        topHorizontal->color = D2D1::ColorF::Aqua;
        bottomHorizontal->color = D2D1::ColorF::Aqua;
    }
protected:
    virtual void Update()
    {
        for (auto eachObj : GetPoolObjects())
            Return(eachObj);

        stack<Collider2D::QuadTreeNode*> nodes;
        nodes.push(Collider2D::QuadTreeNode::rootNode.get());
        while (!nodes.empty())
        {
            auto node = nodes.top();
            nodes.pop();
            if (node == nullptr)
                continue;
            Borrow(node);
            nodes.push(node->leftTop.get());
            nodes.push(node->rightTop.get());
            nodes.push(node->leftBottom.get());
            nodes.push(node->rightBottom.get());
        }
    }

};

