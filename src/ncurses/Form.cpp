#include "Form.hpp"

Form::Form(const int height, const int width, const int toprow, const int leftcol,
           const std::vector<FormFieldParam>& fields_params, const ncurses_string& title)
    : AbstractPostable(height, width, toprow, leftcol, fields_params, title)
{
    // Setup subwindow
    int sub_min_rows, sub_min_cols;
    if(int rc = scale_form(_postable, &sub_min_rows, &sub_min_cols); rc == ERR)
        throw std::runtime_error("scale_form() failed");
    const std::size_t max_label_len =
        std::max_element(fields_params.begin(), fields_params.end(),
                         [](const element_param& a, const element_param& b) {
                             return a.label.len() < b.label.len();
                         })
            ->label.len();
    const int top_shift = _title.empty() ? 0 : 1; // For the title
    const int total_height = sub_min_rows + top_shift;
    const int left_shift = max_label_len;
    const int total_width =
        std::max(static_cast<std::size_t>(sub_min_cols + left_shift), title.len());
    if(height < total_height || width < total_width) {
        std::ostringstream oss;
        oss << "Elements don't fit form windows. Main window height: " << height
            << ", width: " << width << ". Needed height: " << total_height
            << ", width: " << total_width;
        throw std::runtime_error(oss.str());
    }
    init_subwindow(sub_min_rows, sub_min_cols, top_shift, left_shift);

    // Labels
    for(auto& p : fields_params)
        mvwaddstr(_window, p.toprow + top_shift, 0, p.label.str());
    // Fields style
    for(auto& field : _elements)
        if(int rc = set_field_back(field, A_UNDERLINE); rc == ERR)
            throw std::runtime_error("set_field_back() failed");
}

std::vector<std::string> Form::fill_form() const
{
    chtype ch;
    bool loop = true;
    while(loop) {
        ch = get_ch();
        switch(ch) {
        case KEY_DOWN:
            driver(REQ_NEXT_FIELD);
            driver(REQ_END_LINE);
            break;
        case KEY_UP:
            driver(REQ_PREV_FIELD);
            driver(REQ_END_LINE);
            break;
        case KEY_ENTER:
        case 10: // TODO KEY_ENTER doesn't work on my system
            driver(REQ_VALIDATION);
            return field_buffers();
            break;
        case KEY_BACKSPACE:
            driver(REQ_DEL_PREV);
            break;
        default:
            driver(ch);
            break;
        }
    }
    throw std::runtime_error("Invalid Form");
    return {};
}

Form::traits::element_pointer Form::get_field(std::size_t index) const
{
    const std::size_t count = field_count(_postable);
    if(index > count - 1) {
        std::ostringstream oss;
        oss << "Index " << index << " is out of range. Form field count: " << count;
        throw std::out_of_range(oss.str());
    }
    return form_fields(_postable)[index];
}

std::vector<std::string> Form::field_buffers() const
{
    std::vector<std::string> buffers;
    buffers.reserve(_elements.size() - 1);
    for(auto field : _elements)
        if(field != nullptr) {
            std::string str(field_buffer(field, 0));
            // Trim leading and trailing spaces
            static const char space = ' ';
            const std::size_t start = str.find_first_not_of(space);
            std::string trimmed_str("");
            if(start != std::string::npos) /* If any char is not space */ {
                const std::size_t end = str.find_last_not_of(space) + 1;
                trimmed_str = str.substr(start, end - start);
            }
            buffers.push_back(trimmed_str);
        }

    return buffers;
}