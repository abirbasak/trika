#ifndef AZUIK_COMPUTE_SPELLING_HPP
#define AZUIK_COMPUTE_SPELLING_HPP

#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
namespace azuik
{
    namespace compute
    {
        class edit_distance_fn {
        public:
            template <class String, class BackInserter>
            BackInserter operator()(String const& word, BackInserter& edits) const
            {
                for (typename String::size_type i = 0; i != word.size(); ++i)
                {
                    edits.push_back(word.substr(0, i) + word.substr(i + 1)); //deletion
                }
                for (typename String::size_type i = 0; i != word.size() - 1; ++i)
                {
                    edits.push_back(word.substr(0, i) + word[i + 1] + word[i]
                                    + word.substr(i + 2)); //transposition.
                }
                for (char c = 'a'; c != 'z' + 1; ++c)
                {
                    for (typename String::size_type i = 0; i != word.size() + 1; ++i)
                    {
                        edits.push_back(word.substr(0, i) + c + word.substr(i)); //insertion
                    }
                    for (typename String::size_type i = 0; i != word.size(); ++i)
                    {
                        if (c != word[i])
                        {
                            edits.push_back(word.substr(0, i) + c
                                            + word.substr(i + 1)); //replacement
                        }
                    }
                }
                return edits;
            }
        };
        inline const edit_distance_fn edit_distance{};

        class spell_corrector {
        private:
            using freq_type = unsigned long;
            using string_type = std::string;
            using strings_type = std::vector<string_type>;
            using dictionary_type = std::unordered_map<string_type, freq_type>;

        private:
            struct sort_dict {
                bool operator()(dictionary_type ::value_type const& lhs,
                                dictionary_type ::value_type& rhs) const noexcept
                {
                    return lhs.second < rhs.second;
                }
            };

        public:
            explicit spell_corrector(std::string const& dict_file)
            {
                std::ifstream ifs{dict_file.c_str()};
                std::string word;
                freq_type freq;
                while (ifs)
                {
                    ifs >> word >> freq;
                    m_dict.insert(dictionary_type::value_type{word, freq});
                }
            }

            std::string correct(std::string const& word)
            {
                if (m_dict.find(word) != m_dict.end())
                {
                    return word; //word is available in dictionary . return it.
                }
                strings_type edit_dist;
                dictionary_type lexical;
                edit_distance(word, edit_dist);
                filter_known(edit_dist, lexical);
                if (!lexical.empty())
                {
                    //return the best lexical result.
                    return (*std::max_element(lexical.begin(), lexical.end(), sort_dict())).first;
                }
                //else , look for 2 edits
                for (auto& s : edit_dist)
                {
                    strings_type edit_dist2;
                    edit_distance(s, edit_dist2);
                    filter_known(edit_dist2, lexical);
                }
                if (!lexical.empty())
                {
                    //return the best lexical result based on 2 edit distance.
                    return std::get<0>(
                        *std::max_element(lexical.begin(), lexical.end(), sort_dict()));
                }
                //else return the original word. it likely to be OOV
                return word;
            }

        private:
            void filter_known(strings_type const& list, dictionary_type& d) const
            {
                for (auto const& s : list)
                {
                    dictionary_type ::const_iterator w = m_dict.find(s);
                    if (w != m_dict.end())
                    {
                        d[std::get<0>(*w)] = (*w).second;
                    }
                }
            }

        private:
            dictionary_type m_dict;
        };
    } // namespace compute
} // namespace azuik
#endif
