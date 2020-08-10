#include "Calculator.h"

namespace calc {

void Calculate(std::unique_ptr<Expr>& root)
{
	if (root->HasNoChildren() && !root->IsGeneric())
		return;

	if (root->Left() != nullptr && !root->Left()->IsTerminal())
		Calculate(root->Left());

	if (root->Right() != nullptr && !root->Right()->IsTerminal())
		Calculate(root->Right());

	if (!root->IsGeneric())
		CalculateBinNode(root);
	else
	{
		// Calculate children first
		for (int i = 0; i < root->ChildrenSize(); i++)
			Calculate(root->ChildAt(i));

		CalculateGenNode(root);
	}
}

void CalculateBinNode(std::unique_ptr<Expr>& root)
{
	if (root->HasNumberChildren())
	{
		if (root->HasFloatChild())
		{
			float left = std::stof(root->Left()->Name());
			float right = std::stof(root->Right()->Name());

			if (root->IsMul())
				root = std::make_unique<Float>(left * right);
			else if (root->IsAdd())
				root = std::make_unique<Float>(left + right);
			else if (root->IsPow())
				root = std::make_unique<Float>(pow(left, right));
		}
		else
		{
			int left = std::stoi(root->Left()->Name());
			int right = std::stoi(root->Right()->Name());

			if (root->IsMul())
				root = std::make_unique<Integer>(left * right);
			else if (root->IsAdd())
				root = std::make_unique<Integer>(left + right);
			else if (root->IsPow())
				root = std::make_unique<Float>((float)pow(left, right));
		}
	}
}

void CalculateGenNode(std::unique_ptr<Expr>& root)
{
	if (root->IsMul())
		UpdateChildren(root, true);
	else
		UpdateChildren(root, false);
}

void UpdateChildren(std::unique_ptr<Expr>& root, bool isMul)
{
	int first_index = 0;
	int last_index = 0;
	int total_numbers = 0;
	float value = 0.0f;

	if (isMul)
		value = 1.0f;

	for (int i = 0; i < root->ChildrenSize(); i++)
	{
		if (root->ChildAt(i)->IsNumber())
		{
			if (isMul)
			{
				if (value == 1)
					first_index = i;

				value *= stof(root->ChildAt(i)->Name());
			}
			else
			{
				if (value == 0)
					first_index = i;

				value += stof(root->ChildAt(i)->Name());
			}
			total_numbers++;

			if (i == root->ChildrenSize() - 1)
				last_index = i + 1;
			else
				last_index = i;
		}
		else if ((isMul && value != 1) || (!isMul && value != 0))
		{
			last_index = i;
			break;
		}
	}

	if (total_numbers > 1)
		UpdateGenNode(root, value, first_index, last_index);
}

void UpdateGenNode(std::unique_ptr<Expr>& expr, float value, int first_index, int last_index)
{
	bool isInt = false;

	if (abs(value - floor(value)) < 0.000001f)
		isInt = true;

	expr->RemoveChildren(first_index, last_index);

	if (isInt)
		expr->AddChild(std::make_unique<Integer>((int)value));
	else
		expr->AddChild(std::make_unique<Float>(value));

	expr->SortChildren();
}

} //namespace calc
