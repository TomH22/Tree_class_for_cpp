#pragma once
#include "Tree.h"
#include <algorithm>
#include <stdexcept>
#include <map>
#include <string>
#include <vector>

enum ImageType
{
	kMeasuringPoint = 0,
	kWebcam
};

typedef std::vector<std::string> ChildLayerList;

struct Layer
{
	Layer();
	bool operator==(const Layer other) const;
	bool operator!=(const Layer other) const { return !(*this == other); };
	bool operator<(const Layer& other) const;
	bool operator>=(const Layer& other) const { return !(*this < other); }
public:
	bool selected;
	bool checked;
	bool invisible;
	ChildLayerList m_children;
	ChildLayerList m_parent;
	std::string headline;
	std::string note;
	bool markForExport;
	int customLayer;
	std::string name;
};

template <typename BasicJsonType>
static void to_json(BasicJsonType& j, const Layer& layer)
{
	if (layer.name.empty())
		j = nlohmann::json{};
	else
		j = nlohmann::json{
			{
				"layer",
				{
					{"name", layer.name },
					{"headline", layer.headline },
					{"note", layer.note },
					{"markForExport", layer.markForExport},
				}
			}
	};
}

template<typename BasicJsonType>
static void from_json(const BasicJsonType& j, Layer& layer)
{
	if (j.is_null() || j.find("layer") == j.end())
	{
		// nothing
	}
	else
	{
		auto obj = j["layer"];
		layer.name = obj["name"].get<std::string>();
		layer.headline = obj["headline"].get<std::string>();
		layer.note = obj["note"].get<std::string>();
		layer.markForExport = obj["markForExport"].get<bool>();
	}
}

struct TitleblockValue
{
	std::string m_CstrKey;
	std::string m_cstrValue;
	std::string m_CstrOriKey;
};

//////////////////////////////////////////////////////////////////////
// Iteriert wie ProjectExplorer.
class LayerListIterator : public henn::TopDownIterator<Layer>
{
protected:
	struct IteratorOutOfBounds : std::exception
	{
		using std::exception::exception;
	};
public:
	LayerListIterator(henn::TreeModel<Layer>, const bool bMindMarkForExport = false, const std::string csLayerTreeParentPath = "");
	virtual std::string GetLayerName();
	size_t GetIndex() const { return m_lCurrent; };
	virtual void First() override;
	virtual void Next() override;
protected:
	std::string getLayerName(const size_t);
	bool m_bMindMarkForExport;
	std::string m_csLayerTreeParentPath;// für getLayerName(..)
};