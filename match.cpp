#include "match.h"

#include <vector>


namespace MyLib {
    bool match(const std::string &subject, const std::string &mask) {
        const char questionMark = '?';
        const char starMark = '*';

        std::vector<size_t> starIndexes;

        for (size_t i = 0; i < mask.size(); ++i) {
            if (mask[i] == starMark) {
                starIndexes.push_back(i);
            }
        }

        if (starIndexes.empty()) {
            starIndexes.push_back(0);
        }

        std::vector<size_t> starLengthsMax(mask.size(), subject.size());

        for (auto &starIndex : starIndexes) {
            for (size_t i = starIndex + 1; i < mask.size(); ++i) {
                if (mask[i] != starMark) {
                    starLengthsMax[starIndex] -= 1;
                }
            }
        }

        std::vector<size_t> starLengths(mask.size(), 0);
        std::vector<size_t> starCountdowns(mask.size(), 0);

        bool matches = false;
        bool carry = false;
        while (!matches && !carry) {
            for (
                    ;
                    (starLengths[starIndexes.front()] <= starLengthsMax[starIndexes.front()]) && !matches;
                    starLengths[starIndexes.front()]++
                ) {
                matches = true;

                bool starFound = false;
                size_t si = 0;
                size_t mi = 0;
                while (si < subject.size() && mi < mask.size()) {
                    if (mask[mi] == questionMark) {
                        si++;
                        mi++;
                        continue;
                    }

                    if (mask[mi] == starMark) {
                        if (starCountdowns[mi] == 0) {
                            if (starFound) {
                                mi++;
                                starFound = false;
                            } else {
                                starCountdowns[mi] = starLengths[mi];
                                starFound = true;
                            }
                        } else {
                            si++;
                            starCountdowns[mi]--;
                        }

                        continue;
                    }

                    if (mask[mi] == subject[si]) {
                        si++;
                        mi++;
                        continue;
                    } else {
                        matches = false;
                        break;
                    }
                }

                if (si != subject.size()) {
                    matches = false;
                } else {
                    for (; mi < mask.size(); mi++) {
                        if (mask[mi] == starMark) {

                        } else {
                            matches = false;
                            break;
                        }
                    }
                }
            }

            carry = true;
            for (auto &starIndex : starIndexes) {
                if (carry) {
                    starLengths[starIndex]++;
                    carry = (starLengths[starIndex] > starLengthsMax[starIndexes.front()]);
                    if (carry) {
                        starLengths[starIndex] = 0;
                    }
                }
            }
        }

        return matches;
    }
}