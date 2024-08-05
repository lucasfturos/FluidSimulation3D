#pragma once

#include <array>
#include <cmath>
#include <glm/glm.hpp>
#include <memory>
#include <random>

inline double randomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double randomDouble(double min, double max) {
    return min + (max - min) * randomDouble();
}

inline int randomInt(int min, int max) {
    return static_cast<int>(randomDouble(min, max + 1));
}

class Perlin {
  public:
    Perlin() {
        ranvec.resize(point_count);
        for (int i = 0; i < point_count; ++i) {
            ranvec[i] = glm::normalize(glm::vec3(
                randomDouble(-1, 1), randomDouble(-1, 1), randomDouble(-1, 1)));
        }

        perm_x = perlinGeneratePerm();
        perm_y = perlinGeneratePerm();
        perm_z = perlinGeneratePerm();
    }

    double noise(const glm::vec3 &p) const {
        auto u = p.x - std::floor(p.x);
        auto v = p.y - std::floor(p.y);
        auto w = p.z - std::floor(p.z);
        u = u * u * (3 - 2 * u);
        v = v * v * (3 - 2 * v);
        w = w * w * (3 - 2 * w);

        auto i = static_cast<int>(std::floor(p.x));
        auto j = static_cast<int>(std::floor(p.y));
        auto k = static_cast<int>(std::floor(p.z));
        std::array<std::array<std::array<glm::vec3, 2>, 2>, 2> c;

        for (int di = 0; di < 2; ++di) {
            for (int dj = 0; dj < 2; ++dj) {
                for (int dk = 0; dk < 2; ++dk) {
                    c[di][dj][dk] =
                        ranvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^
                               perm_z[(k + dk) & 255]];
                }
            }
        }
        return perlinInterp(c, u, v, w);
    }

    double turb(const glm::vec3 &p, int depth = 7) const {
        auto accum = 0.0;
        auto temp_p = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; ++i) {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p *= 2.0f;
        }

        return std::fabs(accum);
    }

  private:
    static const int point_count = 256;
    std::vector<glm::vec3> ranvec;
    std::vector<int> perm_x;
    std::vector<int> perm_y;
    std::vector<int> perm_z;

    static std::vector<int> perlinGeneratePerm() {
        std::vector<int> p(point_count);
        for (int i = 0; i < point_count; ++i) {
            p[i] = i;
        }
        permute(p);

        return p;
    }

    static void permute(std::vector<int> &p) {
        for (int i = p.size() - 1; i > 0; --i) {
            int target = randomInt(0, i);
            std::swap(p[i], p[target]);
        }
    }

    static double perlinInterp(
        const std::array<std::array<std::array<glm::vec3, 2>, 2>, 2> &c,
        double u, double v, double w) {
        auto uu = u * u * (3 - 2 * u);
        auto vv = v * v * (3 - 2 * v);
        auto ww = w * w * (3 - 2 * w);
        auto accum = 0.0;

        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    glm::vec3 weight_v(u - i, v - j, w - k);
                    accum += (i * uu + (1 - i) * (1 - uu)) *
                             (j * vv + (1 - j) * (1 - vv)) *
                             (k * ww + (1 - k) * (1 - ww)) *
                             dot(c[i][j][k], weight_v);
                }
            }
        }
        return accum;
    }
};