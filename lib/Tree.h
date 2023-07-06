#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <exception>

namespace henn
{
#pragma region TreeModel
/**
Tree structure
@note
	Root item must be set during initalisation.
	Item should define the "<", ">=", "==" and "!=" operators.
**/
template <class Item>
struct TreeModel
{
public:
	struct TreeItem
	{
		Item m_Item;
		std::vector<TreeItem> m_Children;
		bool IsLeaveNode() { return m_Children.size() == 0; };
		bool operator<(const TreeItem& other) const { return (this->m_Item < other.m_Item); };
	};
public:
	TreeModel() = delete;
	/**
	Create the tree model.
	@param _item	root item
	**/
	TreeModel(Item _item) { m_treeItem.m_Item = _item; };
	bool Append(Item, Item);
	Item Get(size_t);
	/**
	Returns root tree node.
	**/
	Item& GetRootItem() const { return m_treeItem.m_Item; }
	TreeItem GetTreeItem() const { return m_treeItem; }
	size_t Size() const;
	/**
	Sort second column from small to big.
	**/
	void SortSecondColumn();
	void SortAllColumns();
private:
	bool addRecursive(TreeItem&, Item&, Item&);
	bool getRecursive(TreeItem&, Item&, size_t, size_t&);
	size_t sizeRecursive(const TreeItem&) const;
	void sortColumnsRecursive(TreeItem&);
private:
	TreeItem m_treeItem;
};

/**
Adds new item to structure.
@param
	_item			Item to add
	_parentItem	Item is added to the _parentItem branch
@return
	true	- item successfull added
	false	- item not successfull added, probabyl parent not found in tree structure
**/
template<typename Item>
bool TreeModel<Item>::Append(Item _item, Item _parentItem)
{
	bool bAdded = false;

	if (m_treeItem.m_Item == _parentItem)
	{
		TreeItem treeItem;
		treeItem.m_Item = _item;
		m_treeItem.m_Children.emplace_back(treeItem);
		bAdded= true;
	}
	else
	{
		for (auto& item : m_treeItem.m_Children)
		{
			bAdded = addRecursive(item, _item, _parentItem);
		}
	}

	return bAdded;
}

/**
Returns item on _index postion.
**/
template<typename Item>
Item TreeModel<Item>::Get(size_t _index)
{
	Item result;

	size_t _nCounter = -1;
	if(!getRecursive(m_treeItem, result, _index, _nCounter))
		throw new std::exception("Index not found!");
	return result;
}

template<class Item>
void TreeModel<Item>::SortSecondColumn()
{
	if (m_treeItem.m_Children.size() < 1)
		return;

	auto& children = m_treeItem.m_Children;
	std::sort(children.begin(), children.end(), [](const TreeItem& a, const TreeItem& b)
		{
			return a < b;
		});
}

/**
Sort second column from small to big.
**/
template<class Item>
void TreeModel<Item>::SortAllColumns()
{
	sortColumnsRecursive(m_treeItem);
}

template<class Item>
void TreeModel<Item>::sortColumnsRecursive(TreeItem &treeItem)
{
	if(treeItem.m_Children.size() < 1)
		return;

	auto& children = treeItem.m_Children;
	std::sort(children.begin(), children.end(), [](const TreeItem& a, const TreeItem& b)
		{
			return a < b;
		});

	std::for_each(children.begin(), children.end(), [this](TreeItem &ti) { sortColumnsRecursive(ti); });
}

template<class Item>
size_t TreeModel<Item>::Size() const
{
	size_t nResult= 1;

	for (auto& treeItem : m_treeItem.m_Children)
	{
		nResult+= sizeRecursive(treeItem);
	}

	return nResult;
}

template<typename Item>
bool TreeModel<Item>::addRecursive(TreeItem& _treeItem, Item& _item, Item& _parentItem)
{
	if (_treeItem.m_Item == _parentItem)
	{
		TreeItem elems;
		elems.m_Item = _item;
		_treeItem.m_Children.emplace_back(elems);
		return true;
	}

	for (auto& layerChilds : _treeItem.m_Children)
	{
		if (addRecursive(layerChilds, _item, _parentItem))
			return true;
	}
	return false;
}

template<typename Item>
size_t TreeModel<Item>::sizeRecursive(const TreeItem& _treeItem) const
{
	size_t _nCounter= 1;
	for (auto treeItem : _treeItem.m_Children)
	{
		_nCounter+= sizeRecursive(treeItem);
	}
	return _nCounter;
};

template<typename Item>
bool TreeModel<Item>::getRecursive(TreeItem& _treeItem, Item& _refItem, size_t _index, size_t& _nCounter)
{
	_nCounter++;

	if(_index == _nCounter)
	{
		_refItem = _treeItem.m_Item;
		return true;
	}

	for(auto& item : _treeItem.m_Children)
	{
		if(getRecursive(item, _refItem, _index, _nCounter))
		{
			return true;
		}
	}

	return false;
};
#pragma endregion
#pragma region Iterators
/**
Iterator base class.
@see Literature
	Design Patterns: Elements of Reusable Object-Oriented Software
	Chapter: BehaviourPatterns / Iterator
**/
template <class Item>
class Iterator {
public:
	virtual void First() = 0;
	virtual void Next() = 0;
	virtual bool IsDone() = 0;
	virtual Item CurrentItem() = 0;
protected:
	Iterator();
};

template<class Item>
inline Iterator<Item>::Iterator()
{
}

/**
Iterates through all elements from top to bottom.
**/
template <class Item>
class TopDownIterator : public Iterator<Item>
{
protected:
	struct IteratorOutOfBounds : std::exception
	{
		using std::exception::exception;
	};
public:
	TopDownIterator(TreeModel<Item>);
	virtual void First();
	virtual void Next();
	virtual bool IsDone();
	virtual Item CurrentItem();
protected:
	size_t m_lCurrent = 0;
	TreeModel<Item> m_tree;
};

/**
Sortiert Layers in eine Baumstruktur.
**/
template<typename Item>
TopDownIterator<Item>::TopDownIterator(TreeModel<Item> tree) : m_tree(tree)
{
}

template<typename Item>
void TopDownIterator<Item>::First()
{
	m_lCurrent = 0;
}

template<typename Item>
void TopDownIterator<Item>::Next()
{
	m_lCurrent++;
}

template<typename Item>
bool TopDownIterator<Item>::IsDone()
{
	return m_lCurrent >= m_tree.Size();
}

template<typename Item>
Item TopDownIterator<Item>::CurrentItem()
{
	//if (IsDone())
	//{
	//	throw IteratorOutOfBounds("No valid state!");
	//}

	return m_tree.Get(m_lCurrent);
}
#pragma endregion
}