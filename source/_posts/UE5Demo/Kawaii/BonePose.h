template<class PoseType>
struct FCSPose
{
    // typedef 告诉编译器PoseType::BoneIndexType是一个类型而不是一个成员
    // 因为模板在实例化之前，编译器并不知道PoseType::BoneIndexType是一个类型还是一个变量
    typedef typename PoseType::BoneIndexType BoneIndexType;

    // 初始化姿势
    void InitPose(const FBoneContainer* InBoneContainer)
    {

    }
};