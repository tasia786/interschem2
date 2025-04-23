#pragma once
#include "structs.h"
#include <raylib.h>

bool CheckCollisionBlocks(const block& shape1, const block& shape2) {
    Rectangle rect1 = { shape1.x - 10, shape1.y - 10, shape1.width + 20, shape1.height + 20 };
    Rectangle rect2 = { shape2.x - 10, shape2.y - 10, shape2.width + 20, shape2.height + 20 }; 
    return CheckCollisionRecs(rect1, rect2);
}

void ResolveCollision(block& shape1, block& shape2) {
    float overlapX = (shape1.x + shape1.width + 10) - (shape2.x - 10);
    float overlapY = (shape1.y + shape1.height + 10) - (shape2.y - 10);

    if (overlapX > 0 && overlapY > 0) {
        if (overlapX < overlapY) {
            shape1.x -= overlapX;
        }
        else {
            shape1.y -= overlapY;
        }
    }
}

bool IsOverlapping(const block& a, const block& b) {
    return !(a.x - 10 + a.width + 20 <= b.x - 10 ||
        a.x - 10 >= b.x + b.width + 20 ||
        a.y - 10 + a.height + 20 <= b.y - 10 ||
        a.y - 10 >= b.y + b.height + 20);
}

bool IsOverlappingAny(const block& newBlock, const vector<block>& shapes, const Rectangle& space) {
    if (newBlock.x - 10 < space.x || newBlock.x + newBlock.width + 10 > space.x + space.width ||
        newBlock.y - 10 < space.y || newBlock.y + newBlock.height + 10 > space.y + space.height) {
        return true;
    }

    for (const auto& shape : shapes) {
        if (IsOverlapping(newBlock, shape)) {
            return true;
        }
    }
    return false;
}

void AvoidOverlapping(block& shape) {
    while (IsOverlappingAny(shape, shapes, space)) {
        shape.x = GetRandomValue(space.x + 10, space.x + space.width - shape.width - 10); 
        shape.y = GetRandomValue(space.y + 10, space.y + space.height - shape.height - 10); 
    }
}
