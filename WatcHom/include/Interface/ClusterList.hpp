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

	void refresh();
	void ButtonCollapse();//with selecting a radioButton
	void ButtonMerge();//enabled only if collabsable is true
private:
	ClusterList();
	std::shared_ptr<std::vector<DGVF::cellBound>> clusters;
	std::vector<sfg::RadioButton::Ptr> radios;
	sfg::Button::Ptr b_merge, b_collapse;
	sfg::Label::Ptr lab_tete;
	sfg::Box::Ptr hb_collapseList;

};

#endif