#if !defined(__static_buffer_string_h__)
#define __static_buffer_string_h__

#include <Arduino.h>
#include <SPIFFS.h>

/** @brief String de buffer estatico, onde o buffer_size define o tamanho do buffer portanto o tamanho maximo da string */
template <size_t buffer_size>
class StaticString
{
protected:
  char buffer[buffer_size];
  size_t len = 0;

public:
  StaticString(const char *str) { set(str); }

  StaticString(String &str) { set(str); }

  StaticString() { len = 0; }

  /**
   * @brief Concatena o buffer a uma String alvo
   * @return true caso sucesso
   */
  bool concat(const String &str)
  {
    if (str.length() + len < buffer_size && strcpy(buffer + len, str.c_str()) != NULL)
    {
      len += str.length();
      return true;
    }
    return false;
  }

  /**
   * @brief Compara o buffer a uma String
   * @return true caso iguais
   */
  bool equals(const String &s) const
  {
    if (s.length() != this->len)
      return false;

    for (size_t i = 0; i < this->len; i++)
    {
      if (buffer[i] != s[i])
        return false;
    }
    return true;
  }

  /**
   * @brief Cria uma copia da string
   * @return Copia da string alvo
   */
  StaticString<buffer_size> &copy() const
  {
    StaticString<buffer_size> rhs = new StaticString<buffer_size>();

    if (strcpy(rhs->buffer, this->buffer) != NULL)
      rhs->len = this->len;

    return (*this);
  }

  /**
   * @brief Atribue um valor ao buffer baseado numa string
   * @return StaticString<buffer_size>
   */
  StaticString<buffer_size> &set(const char *str)
  {
    size_t str_len = strlen(str);
    if (str_len <= buffer_size)
    {
      strcpy(buffer, str);
      len = str_len;
    }
    else
      log_e("String \"%s\" is bigger that current buffer. Buffer size: %u", str, buffer_size);

    return *(this);
  }

  StaticString<buffer_size> &set(String str)
  {
    if (str.length() <= buffer_size)
    {
      strcpy(buffer, str.c_str());
      len = str.length();
    }
    else
      log_e("String \"%s\" is bigger that current buffer. Buffer size: %u", str.c_str(), buffer_size);

    return *(this);
  }

  /** @brief Get content from file if file are smaller or equal the buffer size */
  StaticString<buffer_size> &set_from_file(fs::File file, bool close = true)
  {
    if (file)
    {
      int file_size = file.available();
      if (file_size > 0 && file_size <= buffer_size)
        len = file.readBytes(buffer, file_size);

      if (close)
        file.close();
    }
    else
      log_e("Fail on read file, content not avaliable!");

    return *(this);
  }

  /** @return String com copia do buffer */
  String toString() { return String(buffer).substring(0, len); }

  /** @return storage buffer */
  char *getBuffer() { return this->buffer; }

  /** @return tamanho da string */
  size_t length() { return this->len; };

  const char *c_str(bool noz = false)
  {
    if (!len && noz)
      return NULL;
    else
      return toString().c_str();
  }

  /* Opeadores */
  bool operator==(const String &rhs) const { return equals(rhs); }
  bool operator!=(const String &rhs) const { return !equals(rhs); }
  char operator[](size_t index) { return this->buffer[index]; }
  const char *operator=(StaticString<buffer_size> sstr) { return sstr.c_str(); }
  String operator=(StaticString<buffer_size> &sstr) { return sstr.toString(); }
  StaticString<buffer_size> &operator=(String &str) { return set(str); }
  StaticString<buffer_size> &operator=(const char *cstr) { return set(cstr); }
  StaticString<buffer_size> &operator=(char *str) { return set(String(str)); }
  StaticString<buffer_size> &operator=(fs::File file) { return this->set_from_file(file); }

  String &operator+(const String &str) const { return str + String(buffer); }
  StaticString<buffer_size> &operator+=(const String &str) const
  {
    this->concat(str);
    return (*this);
  }
};

#endif // __static_buffer_string_h__
