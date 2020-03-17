
#include "Parser.h"
#include <memory>


bool Parser::skip_invalid_char(char** buffer_ptr)
{
	char* cur_buffer = *buffer_ptr;

	for (;;)
	{
		char cur_char = *cur_buffer;
		if (CarriageReturn == cur_char || LineFeed == cur_char || Tab == cur_char || Space == cur_char)
		{
			cur_buffer += 1;
		}
		else if ('/' == cur_char) 
		{
			size_t	offset = 1;
			char	next_char = cur_buffer[offset];
			offset += 1;

			if ('/' == next_char)
			{
				for (; LineFeed != cur_buffer[offset]; offset += 1) {}

				cur_buffer += offset;
			}
			else if ('*' == next_char)
			{
				for (; '/*' != *(short*)&cur_buffer[offset]; offset += 1) {}

				cur_buffer += (offset + 2);
			}
			else 
			{
				const char* msg = "Invalid Syntax!";
				int* assertion_ptr = nullptr;
				*assertion_ptr = 1;
			}
		}
		else
		{
			break;
		}

		if (cur_buffer == buffer_end) 
		{
			*buffer_ptr = cur_buffer;

			return false;
		}
	}

	*buffer_ptr = cur_buffer;

	return true;
}


bool Parser::skip_valid_char(char** buffer_ptr)
{
	char* cur_buffer = *buffer_ptr;

	for (;;)
	{
		char cur_char = *cur_buffer;

		if ('"' == cur_char)
		{
			size_t offset{ 1 };

			for (; '"' != cur_buffer[offset]; offset += 1)
			{
				if (buffer_end + 1 == &cur_buffer[offset])
				{
					return false;
				}
			}

			cur_buffer += (offset + 1);

			continue;
		}
		else if (CarriageReturn != cur_char && LineFeed != cur_char && Tab != cur_char && Space != cur_char)
		{
			unsigned short cur_short = *(unsigned short*)&cur_buffer;
			if ('//' != cur_short && '*/' != cur_short)
			{
				++cur_buffer;
				if (buffer_end + 1 == cur_buffer)
				{
					return false;
				}
			}
		}
		else
		{
			*buffer_ptr = cur_buffer;

			return true;
		}

	}


}

// 현재 라인이 끝이면 false
bool Parser::skip_current_line(char** buffer_ptr)
{
	char* cur_buffer{ *buffer_ptr };
	size_t offset{ 0 };
	char* buf_end_next{ this->buffer_end + 1 };

	skip_invalid_char(&cur_buffer);

	for (; LineFeed != cur_buffer[offset]; offset += 1)
	{
		if (buf_end_next == &cur_buffer[offset])
		{
			return false;
		}
	}

	*buffer_ptr = cur_buffer + offset + 1;

	return true;

}

bool Parser::skip_word_if_get_word(char** buffer_ptr, char* out_word_buffer, size_t out_buf_capacity)
{
	char* cur_buffer{ *buffer_ptr };
	char* first_cahr_ptr{ nullptr };
	size_t 	word_length{ 0 };
	if (skip_invalid_char(&cur_buffer))
	{
		first_cahr_ptr = cur_buffer;

		if (skip_valid_char(&cur_buffer))
		{
			*buffer_ptr = cur_buffer;
			word_length = static_cast<size_t>(cur_buffer - first_cahr_ptr);


			if (word_length >= out_buf_capacity)
			{
				word_length = out_buf_capacity - 1; 
			}

			// copy word
			memcpy(out_word_buffer, first_cahr_ptr, word_length);

			out_word_buffer[word_length] = NULL;

			return true;
		}
	}

	return false;
}

Parser::Parser() : buffer{ nullptr }, buffer_end{ nullptr }, buffer_length{ 0 }
{}
Parser::~Parser()
{
	if (nullptr != buffer)
	{
		delete[] buffer;
	}
}


void Parser::load_file(const char* file_name)
{
	FILE* file_ptr{ NULL };
	fopen_s(&file_ptr, file_name, "rb");
	if (NULL == file_ptr)
	{
		int* assertion_ptr = nullptr;
		*assertion_ptr = 0xDEADDEAD;
	}

	fseek(file_ptr, 0, SEEK_END);	
	buffer_length = ftell(file_ptr);      

	fseek(file_ptr, 0, SEEK_SET);	

	// 파일의 내용을 읽어 옴.
	buffer = new char[buffer_length + 1];
	buffer_end = &buffer[buffer_length];
	buffer[buffer_length] = Space;

	fread(buffer, buffer_length, 1, file_ptr);


	fclose(file_ptr);
}

bool Parser::get_bool(const char* field_name, bool& out_param)
{
	char* cur_buffer{ buffer };
	char word_buffer[256]{ };

	for (;;)
	{
		// find lhs
		if (skip_word_if_get_word(&cur_buffer, word_buffer, sizeof(word_buffer)))
		{
			// complare lhs and field_name
			if (0 == strcmp(word_buffer, field_name))
			{
				// 	find '=' .
				if (skip_word_if_get_word(&cur_buffer, word_buffer, sizeof(word_buffer)))
				{
					if ('=' == word_buffer[0])
					{
						// find rhs(value) ;
						if (skip_word_if_get_word(&cur_buffer, word_buffer, sizeof(word_buffer)))
						{
							if (0 == strcmp("false", word_buffer))
							{
								out_param = false;
								return true;
							}
							else if (0 == strcmp("true", word_buffer))
							{
								out_param = true;
								return true;
							}
							else
							{
								int* assert_ptr = nullptr;
								*assert_ptr = 0xDEADDEAD;

							}
							return false;
						}
					}
				}
				else // Syntax Error Assertion
				{
					int* ptr = nullptr;
					*ptr = 0xDEADBEEF;
				}
			}
			else 
			{
				skip_current_line(&cur_buffer);
				continue;
			}
		}

		return false;
	}
}



bool Parser::get_uint(const char* field_name, uint64_t& out_param)
{
	char* cur_buffer{ buffer };
	char word_buffer[256]{ };

	for (;;)
	{
		if (skip_word_if_get_word(&cur_buffer, word_buffer, sizeof(word_buffer)))
		{
			if (0 == strcmp(word_buffer, field_name))
			{
				if (skip_word_if_get_word(&cur_buffer, word_buffer, sizeof(word_buffer)))
				{
					if ('=' == word_buffer[0])
					{
						if (skip_word_if_get_word(&cur_buffer, word_buffer, sizeof(word_buffer)))
						{
							out_param = (size_t)atoll(word_buffer);

							return true;
						}
					}
				}
				else 
				{
					int* ptr = nullptr;
					*ptr = 0xDEADBEEF;
				}
			}
			else 
			{
				skip_current_line(&cur_buffer);
				continue;
			}
		}

		return false;
	}

}

bool Parser::get_float(const char* field_name, float& out_param)
{
	char* cur_buffer{ buffer };
	char	word_buffer[256]{ };

	for (;;)
	{
		if (skip_word_if_get_word(&cur_buffer, word_buffer, sizeof(word_buffer)))
		{
			if (0 == strcmp(word_buffer, field_name))
			{
				if (skip_word_if_get_word(&cur_buffer, word_buffer, sizeof(word_buffer)))
				{
					if ('=' == word_buffer[0])
					{
						if (skip_word_if_get_word(&cur_buffer, word_buffer, sizeof(word_buffer)))
						{
							out_param = atof(word_buffer);

							return true;
						}
					}
				}
				else 
				{
					int* ptr = nullptr;
					*ptr = 0xDEADBEEF;
				}
			}
			else 
			{
				skip_current_line(&cur_buffer);
				continue;
			}
		}

		return false;
	}

}

bool Parser::get_string(const char* field_name, char* out_param, size_t out_buffer_capacity)
{
	char* cur_buffer{ buffer };
	char* first_char_ptr{ nullptr };
	char* last_char_ptr{ nullptr };
	char temp_buffer[256]{	};

	for (;;)
	{
		if (skip_invalid_char(&cur_buffer))
		{
			first_char_ptr = cur_buffer;

			if (skip_valid_char(&cur_buffer))
			{
				size_t lhs_length{ static_cast<size_t>(cur_buffer - first_char_ptr) };

				memcpy(temp_buffer, first_char_ptr, lhs_length);
				temp_buffer[lhs_length] = NULL;

				if (0 == strcmp(temp_buffer, field_name))
				{
					if (skip_invalid_char(&cur_buffer))
					{
						if ('=' == *cur_buffer)
						{
							cur_buffer += 1;
							if (skip_invalid_char(&cur_buffer))
							{
								first_char_ptr = cur_buffer;
								if (skip_valid_char(&cur_buffer))
								{
									size_t rhs_length{ static_cast<size_t>(cur_buffer - first_char_ptr) };

									memcpy(out_param, first_char_ptr + 1, rhs_length - 2);
									out_param[rhs_length - 2] = NULL;

									return true;
								}
							}
						}
						else // Syntax Error Assertion
						{
							int* ptr = nullptr;
							*ptr = 0xDEADBEEF;
						}
					}
				}
				else 
				{
					skip_current_line(&cur_buffer);
					continue;
				}
			}
		}

		return false;
	}
}
