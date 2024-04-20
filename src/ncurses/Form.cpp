#include "Form.hpp"

Form::Form(const int height, const int width, const int toprow, const int leftcol,
           const traits::element_param_container& fields_params, const std::string& title)
    : AbstractPostable(height, width, toprow, leftcol, title), _fields_params_copy(fields_params)
{
    init_elements();
    init_postable();

    // Setup subwindow

    int subwin_height, subwin_width;
    if(int rc = scale_form(_form, &subwin_height, &subwin_width); rc != E_OK)
        throw std::runtime_error("scale_form() failed");
    const int top_shift = _title.empty() ? 0 : 1;
    const int needed_height = subwin_height + top_shift;
    const std::size_t max_label_len =
        std::max_element(_fields_params_copy.begin(), _fields_params_copy.end(),
                         [](const traits::element_param& a, const traits::element_param& b) {
                             return a.label.length() < b.label.length();
                         })
            ->label.length();
    const int left_shift = std::max({
        static_cast<int>(max_label_len),
        static_cast<int>(title.length()) - subwin_width,
        0,
    });
    const int needed_width = subwin_width + left_shift;
    if(height < needed_height || width < needed_width) {
        std::ostringstream oss;
        oss << "Not enough space for form windows. Main window height: " << height
            << ", width: " << width << ". Needed height: " << needed_height
            << ", width: " << needed_width;
        throw std::runtime_error(oss.str());
    }
    init_subwindow(subwin_height, subwin_width, top_shift, left_shift);

    // Labels

    for(auto& p : _fields_params_copy)
        mvwaddstr(_window, p.toprow + top_shift, 0, p.label.c_str());

    // Fields style

    for(auto& field : _fields)
        if(int rc = set_field_back(field, A_UNDERLINE); rc != E_OK)
            throw std::runtime_error("set_field_back() failed");
}

Form::~Form() noexcept
{
    if(_form) {
        if(int rc = traits::free_postable(_form); rc != E_OK)
            log_ncurses_error("free_form", rc);
        _form = nullptr;
    }
    for(auto& field : _fields) {
        if(field) {
            if(int rc = traits::free_element(field); rc != E_OK)
                log_ncurses_error("free_field", rc);
            field = nullptr;
        }
    }
}

void Form::post() const { AbstractPostable::post<decltype(_form)>(_form); }

void Form::unpost() const { AbstractPostable::unpost<decltype(_form)>(_form); }

void Form::driver(const int code) const { AbstractPostable::driver<decltype(_form)>(_form, code); }

void Form::init_elements()
{
    AbstractPostable::init_elements<decltype(_form), decltype(_fields),
                                    decltype(_fields_params_copy)>(_fields, _fields_params_copy);
}

void Form::init_postable()
{
    AbstractPostable::init_postable<decltype(_form), decltype(_fields)>(_form, _fields);
}

void Form::init_subwindow(const int height, const int width, const int rel_top, const int rel_left)
{
    AbstractPostable::init_subwindow<decltype(_form)>(_form, height, width, rel_top, rel_left);
}