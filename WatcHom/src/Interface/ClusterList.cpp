#include "Interface\ClusterList.hpp"

using namespace sfg;

ClusterList::ClusterList()
{
}
void ClusterList::setClusters(std::shared_ptr<std::vector<DGVF::cellBound>> clust) {

}
void ClusterList::ButtonSelect() {

}
ClusterList::Ptr ClusterList::Create() {
	ClusterList::Ptr ret(new ClusterList());
	auto hbox = Box::Create(Box::Orientation::VERTICAL);
	hbox->Pack(Label::Create("machin"));
	ret->Add(hbox);
	return ret;
}
const std::string& ClusterList::GetName() const {
	static const std::string name("ClusterList");
	return name;
}