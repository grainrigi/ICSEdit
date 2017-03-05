template<typename Control, typename ...Args>
inline std::shared_ptr<Control> WindowLayer::CreateSizableControl(Args ...args)
{
	std::shared_ptr<Control> ctl{ new Control(args...) };
	m_sizables.push_back(ctl);
	return ctl;
}