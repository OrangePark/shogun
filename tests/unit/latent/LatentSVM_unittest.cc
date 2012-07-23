#include "MockLatentModel.h"
#include <shogun/classifier/svm/LatentLinearMachine.h>

using namespace shogun;
using ::testing::Return;

TEST(LatentSVM, ctor)
{
	using ::testing::AtLeast;
	using ::testing::Exactly;

	MockCLatentModel* model = new MockCLatentModel();
	int32_t dim = 10, samples = 20;

	ON_CALL(*model, get_dim())
		.WillByDefault(Return(dim));

	ON_CALL(*model, get_num_vectors())
		.WillByDefault(Return(samples));

	EXPECT_CALL(*model, get_dim())
		.Times(Exactly(1));

	EXPECT_CALL(*model, get_num_vectors())
		.Times(Exactly(1));

	CLatentLinearMachine* lsvm = new CLatentLinearMachine(model, 10);

	SG_UNREF(lsvm);
}

TEST(LatentSVM, argmax_h)
{
	using ::testing::AtMost;
	using ::testing::_;

	MockCLatentModel* model = new MockCLatentModel();
	int32_t dim = 10, samples = 20;
	SGVector<float64_t> a(dim);
	CLatentData* data = new CLatentData();

	ON_CALL(*model, get_dim())
		.WillByDefault(Return(dim));

	ON_CALL(*model, get_num_vectors())
		.WillByDefault(Return(samples));

	EXPECT_CALL(*model, infer_latent_variable(_,_))
		.Times(AtMost(samples))
		.WillRepeatedly(Return(data));

	model->argmax_h(a);

	SG_UNREF(data);
	SG_UNREF(model);
}

TEST(LatentSVM, apply)
{
	using ::testing::AtMost;
	using ::testing::_;

	MockCLatentModel* model = new MockCLatentModel();
	int32_t dim = 10, samples = 20;
	SGVector<float64_t> a(dim);
	CLatentData* data = new CLatentData();
	CLatentFeatures* f = new CLatentFeatures(samples);

	ON_CALL(*model, get_dim())
		.WillByDefault(Return(dim));

	ON_CALL(*model, get_num_vectors())
		.WillByDefault(Return(samples));

	EXPECT_CALL(*model, get_num_vectors())
		.Times(2);

	EXPECT_CALL(*model, infer_latent_variable(_,_))
		.Times(samples)
		.WillRepeatedly(Return(data));

	EXPECT_CALL(*model, get_psi_feature_vector(_))
		.Times(samples)
		.WillRepeatedly(Return(a));

	CLatentLinearMachine* lsvm = new CLatentLinearMachine(model, 10);

	lsvm->apply(f);

	SG_UNREF(lsvm);
}

