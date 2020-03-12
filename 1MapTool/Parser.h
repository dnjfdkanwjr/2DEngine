#pragma once

#include <cstdint>
class Parser
{
public:
	Parser();
	~Parser();

	// ½ÇÆÐ½Ã assertion
	void load_file(const char* file_name);

	bool get_bool(const char* field_name, bool& out_param);
	bool get_uint(const char* field_name, uint64_t& out_param);
	bool get_float(const char* field_name, float& out_param);
	bool get_string(const char* field_name, char* out_str_buffer, size_t buffer_capacity);

private:
	bool skip_invalid_char(char** buffer_ptr);
	bool skip_valid_char(char** buffer_ptr);
	bool skip_current_line(char** buffer_ptr);
	bool skip_word_if_get_word(char** buffer_ptr, char* out_word_buffer, size_t out_buf_capacity);

private:
	enum Character
	{
		Tab = 0x09, //'	'
		LineFeed = 0x0A, // '\n'
		CarriageReturn = 0x0D, // '\r'
		Space = 0x20, // ' ' 
	};

private:
	char* buffer;
	char* buffer_end;
	size_t	buffer_length;
};
