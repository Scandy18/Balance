FaceCloud ：
  void FaceCloudJudge
	(queue<FaceCloud*> t,vector<FaceCloud*>result, Vector3d center, float radius)
  t:最初的队列只有最大的面云一个元素，如果球体与该对象的碰撞盒相交，就遍历下一层的小面云并压入队列，除非这个对象已经是最底层的面云。
  result：当这个对象是最底层面云时，如果与小球相交，就将自身压入result，result记录所有与小球相交的底层面云。
  center：球心
  radius：半径

  void FaceJudge
	(vector<Vector3d> t, Vector3d center, float radius)

  用来遍历这个面云所有的面，并将接触面的法向量压入t

（开局定义了最大面云 FaceCloud* maxcloud）