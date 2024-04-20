#include "Form.hpp"

Form::Form(const int height, const int width, const int toprow, const int leftcol,
           const int field_width, const traits::element_param_container& fields_params,
           const std::string& title, bool box)
    : AbstractPostable(height, width, toprow, leftcol, title, box),
      _fields_params_copy(fields_params)
{
    if(field_width >= width)
        throw std::invalid_argument("Field width argument: " + std::to_string(field_width) +
                                    " must not be >= width argument: " + std::to_string(width));

    int field_row = 0;
    for(auto& field_param : _fields_params_copy) {
        field_param._width = field_width;
        field_param._row = field_row++;
    }

    init_elements();
    init_postable();

    // Setup subwindow

    int subwin_height, subwin_width;
    if(int rc = scale_form(_form, &subwin_height, &subwin_width); rc != E_OK)
        throw std::runtime_error("scale_form() failed");
    const std::size_t max_label_width =
        std::max_element(_fields_params_copy.begin(), _fields_params_copy.end(),
                         [](const traits::element_param& a, const traits::element_param& b) {
                             return a.label.length() < b.label.length();
                         })
            ->label.length();
    const int title_height = (_title.empty() ? 0 : 1), title_width = title.length(),
              border_size = (box ? 1 : 0);
    const int top_shift = title_height + border_size;
    const int left_shift = max_label_width + border_size;
    const int needed_height = subwin_height + title_height + border_size * 2;
    const int needed_width =
        std::max(subwin_width, title_width) + max_label_width + border_size * 2;
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
        mvwaddstr(_window, p._row + top_shift, border_size, p.label.c_str());

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