#pragma once
#include <iostream>
#include <vector>
#include "Piece.h"

namespace Shapes {
	// Shape Types
	enum class BlockType {
		NONE = 0,
		O,
		I,
		S,
		Z,
		L,
		J,
		T
	};

	// Rotation Direction
	enum class Rotation {
		Right, Left
	};

	// Shape Offset Positions
	class ShapeValues {
	public:
		ShapeValues() {};
		ShapeValues(int x, int y) {
			m_x = x;
			m_y = y;
		};

		sf::Vector2f operator + (sf::Vector2f other) {
			return sf::Vector2f(other.x + this->m_x, other.y + this->m_y);
		}

		int m_x;
		int m_y;
	};

	typedef const std::vector<ShapeValues> shape;

	// Shape Base Class
	class ShapeBase {
	public:
		virtual ~ShapeBase() {};

		// Type of shape
		BlockType m_type;
		// Current rotation
		int m_currRotation = 0;
		// Total rotations shape has
		int m_totalRotations = 0;

		// Current shape
		std::vector<ShapeValues> m_currShape;

		ShapeColour::Colour m_colour;
	};

	// +X > | -X <
	// +Y V | -Y ^

	/*
	Block Type
	Block Current Rotation

	*/

	// Shape 'O'
	struct ShapeO : ShapeBase {
		ShapeO() {
			m_type = BlockType::O;
			m_totalRotations = 1;
			m_currRotation = 0;
			m_currShape = shape1;
			m_colour = ShapeColour::Colour::YELLOW;
		};

	public:
		shape shape1 = { ShapeValues(0,0), ShapeValues(1,0), ShapeValues(0,-1), ShapeValues(1,-1) };
	};

	// Shape 'I'
	struct ShapeI : ShapeBase {
		ShapeI() {
			m_type = BlockType::I;
			m_totalRotations = 2;
			m_currRotation = 1;
			m_currShape = shape1;
			m_colour = ShapeColour::Colour::CYAN;
		}

	public:
		shape shape1 = { ShapeValues(0,1), ShapeValues(0,0), ShapeValues(0,-1), ShapeValues(0,-2) };
		shape shape2 = { ShapeValues(1,0), ShapeValues(0,0), ShapeValues(-1,0), ShapeValues(-2,0) };
	};

	// Shape 'S'
	struct ShapeS : ShapeBase {
		ShapeS() {
			m_type = BlockType::S;
			m_totalRotations = 2;
			m_currRotation = 1;
			m_currShape = shape1;
			m_colour = ShapeColour::Colour::GREEN;
		}

	public:
		shape shape1 = { ShapeValues(1,0), ShapeValues(0,0), ShapeValues(0,1), ShapeValues(-1,1) };
		shape shape2 = { ShapeValues(0,-1), ShapeValues(0,0), ShapeValues(1,0), ShapeValues(1,1) };
	};

	// Shape 'Z'
	struct ShapeZ : ShapeBase {
		ShapeZ() {
			m_type = BlockType::Z;
			m_totalRotations = 2;
			m_currRotation = 1;
			m_currShape = shape1;
			m_colour = ShapeColour::Colour::RED;
		}

	public:
		shape shape1 = { ShapeValues(-1,0), ShapeValues(0,0), ShapeValues(0,1), ShapeValues(1,1) };
		shape shape2 = { ShapeValues(0,1), ShapeValues(0,0), ShapeValues(1,0), ShapeValues(1,-1) };
	};

	// Shape 'L'
	struct ShapeL : ShapeBase {
		ShapeL() {
			m_type = BlockType::L;
			m_totalRotations = 4;
			m_currRotation = 1;
			m_currShape = shape1;
			m_colour = ShapeColour::Colour::ORANGE;
		}

	public:
		shape shape1 = { ShapeValues(0,-1), ShapeValues(0,0), ShapeValues(0,1), ShapeValues(1,1) };
		shape shape2 = { ShapeValues(-1,0), ShapeValues(0,0), ShapeValues(1,0), ShapeValues(1,-1) };
		shape shape3 = { ShapeValues(0,1), ShapeValues(0,0), ShapeValues(0,-1), ShapeValues(-1,-1) };
		shape shape4 = { ShapeValues(1,0), ShapeValues(0,0), ShapeValues(-1,0), ShapeValues(-1,1) };
	};

	// Shape 'J'
	struct ShapeJ : ShapeBase {
		ShapeJ() {
			m_type = BlockType::J;
			m_totalRotations = 4;
			m_currRotation = 1;
			m_currShape = shape1;
			m_colour = ShapeColour::Colour::BLUE;
		}

	public:
		shape shape1 = { ShapeValues(0,-1), ShapeValues(0,0), ShapeValues(0,1), ShapeValues(-1,1) };
		shape shape2 = { ShapeValues(-1,0), ShapeValues(0,0), ShapeValues(1,0), ShapeValues(1,1) };
		shape shape3 = { ShapeValues(0,1), ShapeValues(0,0), ShapeValues(0,-1), ShapeValues(1,-1) };
		shape shape4 = { ShapeValues(1,0), ShapeValues(0,0), ShapeValues(-1,0), ShapeValues(-1,-1) };
	};

	// Shape 'T'
	struct ShapeT : ShapeBase {
		ShapeT() {
			m_type = BlockType::T;
			m_totalRotations = 4;
			m_currRotation = 1;
			m_currShape = shape1;
			m_colour = ShapeColour::Colour::PURPLE;
		}

	public:
		shape shape1 = { ShapeValues(-1,0), ShapeValues(0,0), ShapeValues(0,1), ShapeValues(1,0) };
		shape shape2 = { ShapeValues(0,1), ShapeValues(0,0), ShapeValues(1,0), ShapeValues(0,-1) };
		shape shape3 = { ShapeValues(1,0), ShapeValues(0,0), ShapeValues(0,-1), ShapeValues(-1,0) };
		shape shape4 = { ShapeValues(0,-1), ShapeValues(0,0), ShapeValues(-1,0), ShapeValues(0,1) };
	};

	// Static Functions for Blocks

	static shape GetShapeFromRotation(ShapeBase& block, const unsigned int rotation) {
		switch(block.m_type) {
		case Shapes::BlockType::O: return block.m_currShape; break;
		case Shapes::BlockType::I: {
			ShapeI* shape = dynamic_cast<ShapeI*>(&block);

			int value = 0;
			if(rotation > shape->m_totalRotations) {
				value = shape->m_totalRotations;
			} else if(rotation < 1) {
				value = 1;
			} else {
				value = rotation;
			}

			switch(value) {
			case 1:	return shape->shape1;
			case 2:	return shape->shape2;
			default: return shape->m_currShape;
			}

			break;
		}
		case Shapes::BlockType::S: {
			ShapeS* shape = dynamic_cast<ShapeS*>(&block);

			int value = 0;
			if(rotation > shape->m_totalRotations) {
				value = shape->m_totalRotations;
			} else if(rotation < 1) {
				value = 1;
			} else {
				value = rotation;
			}

			switch(value) {
			case 1:	return shape->shape1;
			case 2:	return shape->shape2;
			default: return shape->m_currShape;
			}
		}
		case Shapes::BlockType::Z: {
			ShapeZ* shape = dynamic_cast<ShapeZ*>(&block);

			int value = 0;
			if(rotation > shape->m_totalRotations) {
				value = shape->m_totalRotations;
			} else if(rotation < 1) {
				value = 1;
			} else {
				value = rotation;
			}

			switch(value) {
			case 1:	return shape->shape1;
			case 2:	return shape->shape2;
			default: return shape->m_currShape;
			}
		}
		case Shapes::BlockType::L: {
			ShapeL* shape = dynamic_cast<ShapeL*>(&block);

			int value = 0;
			if(rotation > shape->m_totalRotations) {
				value = shape->m_totalRotations;
			} else if(rotation < 1) {
				value = 1;
			} else {
				value = rotation;
			}

			switch(value) {
			case 1:	return shape->shape1;
			case 2:	return shape->shape2;
			case 3: return shape->shape3;
			case 4: return shape->shape4;
			default: return shape->m_currShape;
			}
		}
		case Shapes::BlockType::J: {
			ShapeJ* shape = dynamic_cast<ShapeJ*>(&block);

			int value = 0;
			if(rotation > shape->m_totalRotations) {
				value = shape->m_totalRotations;
			} else if(rotation < 1) {
				value = 1;
			} else {
				value = rotation;
			}

			switch(value) {
			case 1:	return shape->shape1;
			case 2:	return shape->shape2;
			case 3: return shape->shape3;
			case 4: return shape->shape4;
			default: return shape->m_currShape;
			}
		}
		case Shapes::BlockType::T: {
			ShapeT* shape = dynamic_cast<ShapeT*>(&block);

			int value = 0;
			if(rotation > shape->m_totalRotations) {
				value = shape->m_totalRotations;
			} else if(rotation < 1) {
				value = 1;
			} else {
				value = rotation;
			}

			switch(value) {
			case 1:	return shape->shape1;
			case 2:	return shape->shape2;
			case 3: return shape->shape3;
			case 4: return shape->shape4;
			default: return shape->m_currShape;
			}
		}
		}
	}

	static void SetRotation(ShapeBase& block, const unsigned int rotation) {
		if(rotation > block.m_totalRotations) {
			block.m_currRotation = block.m_totalRotations;
		} else if(rotation < 1) {
			block.m_currRotation = 1;
		} else {
			block.m_currRotation = rotation;
		}

		block.m_currShape = GetShapeFromRotation(block, block.m_currRotation);
	}

	static void RotateBlock(ShapeBase& block, Rotation rotate) {
		switch (block.m_type) {
		case BlockType::I:
		{
			ShapeI* shape = dynamic_cast<ShapeI*>(&block);

			++block.m_currRotation;
			if (block.m_currRotation > block.m_totalRotations) {
				block.m_currRotation = 1;
				block.m_currShape = shape->shape1;
				break;
			}

			switch (shape->m_currRotation) {
			case 1: block.m_currShape = shape->shape1; break;
			case 2: block.m_currShape = shape->shape2; break;
			}

			break;
		}
		case BlockType::O:
		{
			// Shape 0 doesnt need rotations
			break;
		}
		case BlockType::L: 
		{
			ShapeL* shape = dynamic_cast<ShapeL*>(&block);

			if (rotate == Rotation::Right)
				++block.m_currRotation;
			else
				--block.m_currRotation;

			if (block.m_currRotation > block.m_totalRotations) {
				block.m_currRotation = 1;
				block.m_currShape = shape->shape1;
				break;
			} else if (block.m_currRotation < 1) {
				block.m_currRotation = block.m_totalRotations;
				block.m_currShape = shape->shape4;
				break;
			}

			switch (shape->m_currRotation) {
			case 1: block.m_currShape = shape->shape1; break;
			case 2: block.m_currShape = shape->shape2; break;
			case 3: block.m_currShape = shape->shape3; break;
			case 4: block.m_currShape = shape->shape4; break;
			}

			break;
		}
		case BlockType::J:
		{
			ShapeJ* shape = dynamic_cast<ShapeJ*>(&block);

			if (rotate == Rotation::Right)
				++block.m_currRotation;
			else
				--block.m_currRotation;

			if (block.m_currRotation > block.m_totalRotations) {
				block.m_currRotation = 1;
				block.m_currShape = shape->shape1;
				break;
			} else if (block.m_currRotation < 1) {
				block.m_currRotation = block.m_totalRotations;
				block.m_currShape = shape->shape4;
				break;
			}

			switch (shape->m_currRotation) {
			case 1: block.m_currShape = shape->shape1; break;
			case 2: block.m_currShape = shape->shape2; break;
			case 3: block.m_currShape = shape->shape3; break;
			case 4: block.m_currShape = shape->shape4; break;
			}

			break;
		}
		case BlockType::S: 
		{
			ShapeS* shape = dynamic_cast<ShapeS*>(&block);

			++block.m_currRotation;
			if (block.m_currRotation > block.m_totalRotations) {
				block.m_currRotation = 1;
				block.m_currShape = shape->shape1;
				break;
			}

			switch (shape->m_currRotation) {
			case 1: block.m_currShape = shape->shape1; break;
			case 2: block.m_currShape = shape->shape2; break;
			}

			break;
		}
		case BlockType::Z: 
		{
			ShapeZ* shape = dynamic_cast<ShapeZ*>(&block);

			++block.m_currRotation;
			if (block.m_currRotation > block.m_totalRotations) {
				block.m_currRotation = 1;
				block.m_currShape = shape->shape1;
				break;
			}

			switch (shape->m_currRotation) {
			case 1: block.m_currShape = shape->shape1; break;
			case 2: block.m_currShape = shape->shape2; break;
			}

			break;
		}
		case BlockType::T: 
		{
			ShapeT* shape = dynamic_cast<ShapeT*>(&block);

			if (rotate == Rotation::Right)
				++block.m_currRotation;
			else
				--block.m_currRotation;

			if (block.m_currRotation > block.m_totalRotations) {
				block.m_currRotation = 1;
				block.m_currShape = shape->shape1;
				break;
			} else if (block.m_currRotation < 1) {
				block.m_currRotation = block.m_totalRotations;
				block.m_currShape = shape->shape4;
				break;
			}

			switch (shape->m_currRotation) {
			case 1: block.m_currShape = shape->shape1; break;
			case 2: block.m_currShape = shape->shape2; break;
			case 3: block.m_currShape = shape->shape3; break;
			case 4: block.m_currShape = shape->shape4; break;
			}

			break;
		}
		default:
			// Do something here?
			break;
		}
	}

	static shape GetNextRotation(ShapeBase& block, Rotation rotate) {
		switch (block.m_type) {
		case BlockType::I:
		{
			ShapeI* shape = dynamic_cast<ShapeI*>(&block);
			switch (block.m_currRotation) {
			case 1: return shape->shape2;
			case 2: return shape->shape1;
			}
		}
		case BlockType::J:
		{
			ShapeJ* shape = dynamic_cast<ShapeJ*>(&block);
			switch (block.m_currRotation) {
			case 1: return (rotate == Rotation::Right) ? shape->shape2 : shape->shape4;
			case 2: return (rotate == Rotation::Right) ? shape->shape1 : shape->shape3;
			case 3: return (rotate == Rotation::Right) ? shape->shape2 : shape->shape4;
			case 4: return (rotate == Rotation::Right) ? shape->shape3 : shape->shape1;
			}
		}
		case BlockType::O: return block.m_currShape;
		case BlockType::S:
		{
			ShapeS* shape = dynamic_cast<ShapeS*>(&block);
			switch (block.m_currRotation) {
			case 1: return shape->shape2;
			case 2: return shape->shape1;
			}
			break;
		}
		case BlockType::Z:
		{
			ShapeZ* shape = dynamic_cast<ShapeZ*>(&block);
			switch (block.m_currRotation) {
			case 1: return shape->shape2;
			case 2: return shape->shape1;
			}
			break;
		}
		case BlockType::L:
		{
			ShapeL* shape = dynamic_cast<ShapeL*>(&block);
			switch (block.m_currRotation) {
			case 1: return (rotate == Rotation::Right) ? shape->shape2 : shape->shape4;
			case 2: return (rotate == Rotation::Right) ? shape->shape1 : shape->shape3;
			case 3: return (rotate == Rotation::Right) ? shape->shape2 : shape->shape4;
			case 4: return (rotate == Rotation::Right) ? shape->shape3 : shape->shape1;
			}
		}
		case BlockType::T:
		{
			ShapeT* shape = dynamic_cast<ShapeT*>(&block);
			switch (block.m_currRotation) {
			case 1: return (rotate == Rotation::Right) ? shape->shape2 : shape->shape4;
			case 2: return (rotate == Rotation::Right) ? shape->shape1 : shape->shape3;
			case 3: return (rotate == Rotation::Right) ? shape->shape2 : shape->shape4;
			case 4: return (rotate == Rotation::Right) ? shape->shape3 : shape->shape1;
			}
		}
		}
	}

	static ShapeBase* GetRandomBlock() {
		int randNum = rand() % 7 + 1;

		switch ((BlockType)randNum) {
		case BlockType::O: return new ShapeO;
		case BlockType::I: return new ShapeI;
		case BlockType::S: return new ShapeS;
		case BlockType::Z: return new ShapeZ;
		case BlockType::L: return new ShapeL;
		case BlockType::J: return new ShapeJ;
		case BlockType::T: return new ShapeT;
		default:
			printf("Error, Generated %i", randNum);
			return new ShapeI;
		}
	}

	static sf::Vector2f MostRightPosition(ShapeBase block) {
		sf::Vector2f highestPos = sf::Vector2f(-100, 0);
		for (int i = 0; i < 4; i++) {
			if (block.m_currShape[i].m_x > highestPos.x) {
				highestPos = sf::Vector2f(block.m_currShape[i].m_x, block.m_currShape[i].m_y);
			}
		}

		return highestPos;
	}

	static sf::Vector2f MostLeftPosition(ShapeBase block) {
		sf::Vector2f lowestPos = sf::Vector2f(100, 0);
		for (int i = 0; i < 4; i++) {
			if (block.m_currShape[i].m_x < lowestPos.x) {
				lowestPos = sf::Vector2f(block.m_currShape[i].m_x, block.m_currShape[i].m_y);
			}
		}

		return lowestPos;
	}

	static sf::Vector2f LowestPosition(ShapeBase block) {
		sf::Vector2f lowestPos = sf::Vector2f(0, -100);
		for (int i = 0; i < 4; i++) {
			if (block.m_currShape[i].m_y > lowestPos.y) {
				lowestPos = sf::Vector2f(block.m_currShape[i].m_x, block.m_currShape[i].m_y);
			}
		}

		return lowestPos;
	}
}