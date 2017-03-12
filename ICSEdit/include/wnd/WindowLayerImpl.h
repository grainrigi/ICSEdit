
template<typename Control, typename ...Args>
inline std::shared_ptr<Control> ICSE::wnd::WindowLayer::CreateControl(Args ...args)
{
	static_assert(!std::is_base_of<ICSE::wnd::AWCanvasControl, Control>::value, "CanvasControl must be created by CreateAscentCanvasControl or CreateDescentCanvasControl.");

	return std::shared_ptr<Control>();
}

template<typename Control, typename ...Args>
inline std::shared_ptr<Control> WindowLayer::CreateAscentCanvasControl(Args ...args)
{
	static_assert(std::is_base_of<ICSE::wnd::AWCanvasControl, Control>::value, "The Control must inherit AWCanvasControl.");

	std::shared_ptr<Control> ctl{ new Control(args...) };
	m_controls.insert(std::make_pair(ctl->id(), ctl));
	m_ascent.AddCanvasControl(ctl);
	return ctl;
}

template<typename Control, typename ...Args>
inline std::shared_ptr<Control> ICSE::wnd::WindowLayer::CreateDescentCanvasControl(Args ...args)
{
	static_assert(std::is_base_of<ICSE::wnd::AWCanvasControl, Control>::value, "The Control must inherit AWCanvasControl.");

	std::shared_ptr<Control> ctl{ new Control(args) };
	m_controls.insert(ctl->id(), ctl);
	m_descent.AddCanvasControl(ctl);
	return std::shared_ptr<Control>();
}
