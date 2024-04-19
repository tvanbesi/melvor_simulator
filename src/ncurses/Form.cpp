#include "Form.hpp"

Form::Form(const int height, const int width, const int toprow, const int leftcol,
           const traits::element_param_container& fields_params, const std::string& title)
    : AbstractPostable(height, width, toprow, leftcol, title), _fields_params_copy(fields_params)
{
    init_elements();
    init_postable();

    // Setup subwindow
    int sub_min_rows, sub_min_cols;
    if(int rc = scale_form(_form, &sub_min_rows, &sub_min_cols); rc == ERR)
        throw std::runtime_error("scale_form() failed");
    const std::size_t max_label_len =
        std::max_element(_fields_params_copy.begin(), _fields_params_copy.end(),
                         [](const traits::element_param& a, const traits::element_param& b) {
                             return a.label.length() < b.label.length();
                         })
            ->label.length();
    const int top_shift = _title.empty() ? 0 : 1; // For the title
    const int total_height = sub_min_rows + top_shift;
    const int left_shift = max_label_len;
    const int total_width =
        std::max(static_cast<std::size_t>(sub_min_cols + left_shift), title.length());
    if(height < total_height || width < total_width) {
        std::ostringstream oss;
        oss << "Elements don't fit form windows. Main window height: " << height
            << ", width: " << width << ". Needed height: " << total_height
            << ", width: " << total_width;
        throw std::runtime_error(oss.str());
    }
    init_subwindow(sub_min_rows, sub_min_cols, top_shift, left_shift);

    // Labels
    for(auto& p : _fields_params_copy)
        mvwaddstr(_window, p.toprow + top_shift, 0, p.label.c_str());
    // Fields style
    for(auto& field : _fields)
        if(int rc = set_field_back(field, A_UNDERLINE); rc == ERR)
            throw std::runtime_error("set_field_back() failed");
}

Form::~Form() noexcept
{
    if(_form) {
        traits::free_postable(_form);
        _form = nullptr;
    }
    for(auto& field : _fields) {
        if(field) {
            traits::free_element(field);
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