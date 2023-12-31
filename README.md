# [Mocha] | Monte Carlo Integration Solver |

*A small numeric multi-dimensional integral solver based in C++.*

--- 

#### Mocha aims to be:
- a learning exercise for implementing monte carlo methods.
- performant, no one likes slow code.
- useable, interfacing with the program should be easy.
- extensible, provide an easy interface for new features.

#### Features:
- Built with [`meson`](https://github.com/mesonbuild/meson).
- Support for multiple sampling and variance-reduction techniques.
    - Uniform Random Sampling.
    - Latin Hypercube Sampling.
- Loading of integrands, parameters, etc from JSON ([`nlohmann/json`](https://github.com/nlohmann/json)).
- Fast parsing of integrand strings via [`muparser`](https://github.com/beltoforion/muparser).

#### Roadmap:
- Write output to log.
- Stopping conditions for minimum error, maximum calculation time.
- New sampling methods : [Stratified, antithetical, etc].
- Queue multiple jobs.
