#include "Layer.h"

using namespace std;
using namespace henn;

/*
std::string CreateCompressedGuidString()
{
	GUID guid;
	HRESULT hr = CoCreateGuid(&guid);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create GUID");
	}

	// Convert GUID to string
	//return std::format("{:08X}{:04X}{:04X}{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}",
	//	guid.Data1, guid.Data2, guid.Data3,
	//	guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
	//	guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	return std::format("{:08X}{:04X}{:04X}{:02X}{:02X}", guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1]);
}
*/

Layer::Layer()
	: selected(false),
	checked(false),
	invisible(false),
	m_children(),
	customLayer(-1),
	nameUnique("")
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

	if (!nameUnique.empty() && nameUnique != other.nameUnique)
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