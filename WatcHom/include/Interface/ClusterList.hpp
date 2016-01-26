#ifndef CLUSTER_LIST_HPP
#define CLUSTER_LIST_HPP

#include "SFGUI\Frame.hpp"
#include "SFGUI\Widgets.hpp"
#include "Engine\DGVF.hpp"
#include <memory>

class ClusterList : public sfg::Frame
{
public:
	typedef std::shared_ptr<ClusterList> Ptr;
	static Ptr Create();
	~ClusterList() = default;
	const std::string& GetName() const override;

	void setClusters(std::shared_ptr<std::vector<DGVF::cellBound>> clust);
	void ButtonSelect();//when selecting a radioButton
private:
	ClusterList();
	std::shared_ptr<std::vector<DGVF::cellBound>> clusters;
};

#endif