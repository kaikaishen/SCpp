#include "activeModel.hpp"
#include "ecosWrapper.hpp"
#include "discretization.hpp"
#include "SCvxProblem.hpp"
#include "parameterServer.hpp"

namespace scpp
{

class SCvxAlgorithm
{
public:
    /**
     * @brief Construct a new SC solver.
     * 
     * @param model     The system model.
     */
    explicit SCvxAlgorithm(Model::ptr_t model);

    /**
     * @brief Initializes the algorithm. Has to be called before solving the problem.
     * 
     */
    void initialize();

    /**
     * @brief Solves the system.
     * 
     * @param warm_start    Whether to reuse the last computed trajectory.
     */
    void solve(bool warm_start = false);

    /**
     * @brief Get the solution variables object.
     * 
     * @param X     The state trajectory.
     * @param U     The input trajectory.
     * @param t     The final time.
     */
    void getSolution(Model::state_vector_v_t &X, Model::input_vector_v_t &U, double &t);

    /**
     * @brief Get the solution from each iteration
     * 
     * @param X 
     * @param U 
     */
    void getAllSolutions(std::vector<Model::state_vector_v_t> &X,
                         std::vector<Model::input_vector_v_t> &U,
                         std::vector<double> &t);

    double getNonlinearCost();

private:
    /**
     * @brief Saves solution indices for performance.
     * 
     */
    void cacheIndices();

    /**
     * @brief Reads the solution variables X, U.
     * 
     */
    void readSolution();

    /**
     * @brief Loads the parameters from the configuration file.
     * 
     */
    void loadParameters();

    /**
     * @brief Performs a Successive Convexification iteration.
     * 
     */
    bool iterate();

    size_t K;

    Model::ptr_t model;

    bool interpolate_input;

    bool nondimensionalize;
    double alpha;
    double beta;
    double rho_0;
    double rho_1;
    double rho_2;
    double change_threshold;

    double trust_region;
    double weight_virtual_control;
    size_t max_iterations;
    std::optional<double> last_nonlinear_cost;

    Model::state_matrix_v_t A_bar;
    Model::control_matrix_v_t B_bar;
    Model::control_matrix_v_t C_bar;
    Model::state_vector_v_t z_bar;

    double sigma;
    Model::state_vector_v_t X;
    Model::input_vector_v_t U;

    std::vector<Model::state_vector_v_t> all_X;
    std::vector<Model::input_vector_v_t> all_U;

    Model::state_vector_v_t old_X;
    Model::input_vector_v_t old_U;

    Eigen::MatrixXi X_indices;
    Eigen::MatrixXi U_indices;

    op::SecondOrderConeProgram socp;

    std::unique_ptr<EcosWrapper> solver;
};

} // namespace scpp