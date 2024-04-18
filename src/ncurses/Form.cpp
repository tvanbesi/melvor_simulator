#include "Form.hpp"

Form::Form(const int height, const int width, const int toprow, const int leftcol,
           const std::vector<FormFieldParam>& fields_params)
    : AbstractPostable(height, width, toprow, leftcol, fields_params)
{
    // Setup subwindow
    int sub_min_rows, sub_min_cols;
    if(int rc = scale_form(_postable, &sub_min_rows, &sub_min_cols); rc == ERR)
        throw std::runtime_error("scale_form() failed");
    const std::size_t max_label_len =
        std::strlen(std::max_element(fields_params.begin(), fields_params.end(),
                                     [](const element_param& a, const element_param& b) {
                                         return std::strlen(a.label) < std::strlen(b.label);
                                     })
                        ->label);
    const int total_height = sub_min_rows;
    const int left_shift = max_label_len;
    const int total_width = sub_min_cols + left_shift;
    if(height < total_height || width < total_width) {
        std::ostringstream oss;
        oss << "Labels and fields don't fit form windows. Main window height: " << height
            << ", width: " << width << ". Needed height: " << total_height
            << ", width: " << total_width;
        throw std::runtime_error(oss.str());
    }
    init_subwindow(sub_min_rows, sub_min_cols, 0, left_shift);

    // Write labels
    for(auto& p : fields_params)
        mvwaddstr(_window, p.toprow, 0, p.label);

    // Setup field style
    for(auto& field : _elements)
        if(int rc = set_field_back(field, A_UNDERLINE); rc == ERR)
            throw std::runtime_error("set_field_back() failed");
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