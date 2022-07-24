//from SHOGI_GOGO


int main() {
	/*
	mgr->addSphereSceneNode(1);
	ISceneNode* k1 = mgr->addSphereSceneNode(0.2);
	k1->setMaterialFlag(EMF_LIGHTING, false);
	ISceneNodeAnimator* k1a1 = new CSceneNodeAnimatorFlyCircleRotation(X, vector3df(), 1, 8000, 5000);
	if (k1a1) {
		k1->addAnimator(k1a1);
	}
	ISceneNodeAnimator* k1a2 = new CSceneNodeAnimatorFlyCircleRotation(Y, vector3df(), 1, 5000);
	if (k1a2) {
		k1->addAnimator(k1a2);
	}
	ISceneNode* k2 = mgr->addSphereSceneNode(0.2);
	k2->setMaterialFlag(EMF_LIGHTING, false);
	ISceneNodeAnimator* k2a1 = new CSceneNodeAnimatorFlyCircleRotation(Y, vector3df(), 1, 10000);
	if (k2a1) {
		k2->addAnimator(k2a1);
	}
	ISceneNodeAnimator* k2a2 = new CSceneNodeAnimatorFlyCircleRotation(Z, vector3df(), 1, 4000, 1000);
	if (k2a2) {
		k2->addAnimator(k2a2);
	}
	*/

	/*for (s32 i = 1; i <= 10; i++) {
		Piece* piece = new Piece(mgr->getRootSceneNode(), mgr, 666 + i, (f32)(i * 0.5));
		f32 radius = piece->getRadius();
		piece->setPosition(vector3df(0, (f32)(i * 2), 0));

		ISceneNodeAnimator* flyCircleRotation = new CSceneNodeAnimatorFlyCircleRotation(Y, vector3df(), radius, 10000 + 1000 * i, i * 1000);
		if (flyCircleRotation) {
			piece->addAnimator(flyCircleRotation);
			piece->setRotation(vector3df(0, 90, 0));
		}

		for (s32 j = 0; j < 20; j++) {
			Piece* circle = new Piece(mgr->getRootSceneNode(), mgr, -1, (f32)(i * 0.5));
			f32 degree = (f32)(j * 18);
			vector3df cirpos(0, (f32)(i * 2), radius);
			cirpos.rotateXZBy(degree);
			circle->setPosition(cirpos);
			circle->setRotation(vector3df(0, -degree, 0));
		}

	}*/
}