#include "Layer.h"

using namespace std;
using namespace henn;

Layer::Layer()
	: selected(false),
	checked(false),
	invisible(false),
	m_children(),
	customLayer(-1)
{
}

bool Layer::operator<(const Layer& other) const
{
	auto nameLit = name;
	auto otherNameLit = other.name;

	return (nameLit < otherNameLit);
}

bool Layer::operator==(const Layer other) const
{
	//if (m_parent.size() != other.m_parent.size())
	//	return false;

	//for (int i = 0; i < m_parent.size(); i++)
	//{
	//	if (m_parent[i] != other.m_parent[i])
	//		return false;
	//}

	if (name != other.name)
		return false;

	return true;
};

//////////////////////////////////////////////////////////////////////
// Sortiert Layers in eine Baumstruktur.
LayerListIterator::LayerListIterator(TreeModel<Layer> treeModel, const bool bMindMarkForExport, const string csLayerTreeParentPath) :
	TopDownIterator{ treeModel },
	m_bMindMarkForExport(bMindMarkForExport),
	m_csLayerTreeParentPath(csLayerTreeParentPath)
{
}

void LayerListIterator::First()
{
	if (!m_bMindMarkForExport)
	{
		TopDownIterator::First();
	}
	else
	{
		m_lCurrent = 0;

		for (; m_lCurrent < m_tree.Size(); m_lCurrent++)
		{
			if (m_tree.Get(m_lCurrent).markForExport)
			{
				break;
			}
		}
	}
}

void LayerListIterator::Next()
{
	if (!m_bMindMarkForExport)
	{
		TopDownIterator::Next();
	}
	else
	{
		m_lCurrent++;
		for (; m_lCurrent < m_tree.Size(); m_lCurrent++)
		{
			if (m_tree.Get(m_lCurrent).markForExport)
			{
				break;
			}
		}
	}
}

string LayerListIterator::GetLayerName()
{
	return getLayerName(m_lCurrent);
}

string LayerListIterator::getLayerName(const size_t index)
{
	string result;
	result += m_csLayerTreeParentPath;

	auto path = m_tree.GetPath(index);
	// lazy solution
	auto size = (CurrentItem().customLayer != 1) ? path.size() : path.size() - 1;
	for (size_t i = 0; i < size; i++)
	{
		result += "." + path[i].name;
	}

	return result;
}