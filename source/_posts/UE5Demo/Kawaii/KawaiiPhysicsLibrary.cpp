class FTransform;

class UKawaiiPhysicsLibrary
{
public:
    /**
     * 骨骼是否在盒子里面
    */
    bool IsBoneInBox(const FTransform& BoneTransform, const FVector& BoxLocation, const FVector& BoxScale, const FQuat& BoxRotationInverse);    
};