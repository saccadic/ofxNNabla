#pragma once

/* https://github.com/sony/nnabla/tree/master/examples/cpp/mnist_runtime */

#include <nbla_utils/nnp.hpp>

#ifdef WITH_CUDA
#include <nbla/cuda/cudnn/init.hpp>
#include <nbla/cuda/init.hpp>
#endif

namespace ofxnnabla {
	enum RUN {
		CPU,
		GPU
	};

	template<typename T>
	class NNablaRuntime {
	public:

		RUN mode = RUN::CPU;

		void Init(RUN _mode) {
			mode = _mode;
			ctx_cpu = nbla::Context{ {"cpu:float"}, "CpuCachedArray", "0" };
			if (mode == RUN::CPU) {
				nnp = new nbla::utils::nnp::Nnp(ctx_cpu);
			}
			else {
#ifdef WITH_CUDA
				nbla::init_cudnn();
				ctx_gpu = nbla::Context{{"cudnn:float", "cuda:float", "cpu:float"}, "CudaCachedArray", "0" };
				nnp = new nbla::utils::nnp::Nnp(ctx_gpu);
#endif
			}
		}

		bool LoadModel(string path, string executorName) {
			if (nnp->add(path)) {
				executor = nnp->get_executor(executorName);
				executor->set_batch_size(1);

				cout << executor->network_name() << endl;

				return true;
			}
			else {
				return false;
			}
		}

		T* GetInputArrayPtr(int id) {
			nbla::CgVariablePtr x = executor->get_data_variables().at(id).variable;
			return x->variable()->cast_data_and_get_pointer<float>(ctx_cpu);
		}

		void Run() {
			executor->execute();
		}

		const T* GetOutputArrayPtr(int id) {
			nbla::CgVariablePtr y = executor->get_output_variables().at(id).variable;
			return y->variable()->get_data_pointer<float>(ctx_cpu);
		}

	private:
		nbla::Context ctx_cpu;
		nbla::Context ctx_gpu;
		nbla::utils::nnp::Nnp *nnp;
		shared_ptr<nbla::utils::nnp::Executor> executor;
	};
}