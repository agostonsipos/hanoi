#include <osg/Texture2D>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osg/MatrixTransform>

#ifdef _DEBUG
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "osgDBd.lib")
#pragma comment(lib, "osgViewerd.lib")
#else
#pragma comment(lib, "osg.lib")
#pragma comment(lib, "osgDB.lib")
#pragma comment(lib, "osgViewer.lib")
#endif

#pragma warning(disable : 4482 )

template <bool f = false>
void calc(float r, float x, float y,
	osg::ref_ptr<osg::Vec3Array>& vertices,
	osg::ref_ptr<osg::Vec3Array>& normals,
	osg::ref_ptr<osg::Vec2Array>& texcoords
	){
	float u = 2 * osg::PI * x;
	float v = y;
	vertices->push_back(osg::Vec3(r*cos(u), v, r*sin(u)));
	normals->push_back(osg::Vec3(f?-cos(u):cos(u), 0, f?-sin(u):sin(u)));
	texcoords->push_back(osg::Vec2(x, y));
}

template <bool f>
void calc2(float r, float x, float y,
	osg::ref_ptr<osg::Vec3Array>& vertices,
	osg::ref_ptr<osg::Vec3Array>& normals,
	osg::ref_ptr<osg::Vec2Array>& texcoords
	){
	float u = 2 * osg::PI * x;
	vertices->push_back(osg::Vec3(r*cos(u), y, r*sin(u)));
	normals->push_back(osg::Vec3(0,f?1:-1,0));
	texcoords->push_back(osg::Vec2(r*cos(u)/6+0.5, r*sin(u)/6+0.5));
}

int main(int argc, char** argv)
{
	atexit([](){std::cin.get(); });

	// pozíciók
	osg::ref_ptr<osg::Vec3Array> vertices1 = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> normals1 = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> texcoords1 = new osg::Vec2Array;


	osg::ref_ptr<osg::Vec3Array> points = new osg::Vec3Array;
	points->push_back(osg::Vec3(0, 0, 0));
	points->push_back(osg::Vec3(1, 0, 0));
	points->push_back(osg::Vec3(0, 0, 1));
	points->push_back(osg::Vec3(1, 0, 1));
	points->push_back(osg::Vec3(0, 1, 0));
	points->push_back(osg::Vec3(1, 1, 0));
	points->push_back(osg::Vec3(0, 1, 1));
	points->push_back(osg::Vec3(1, 1, 1));


	vertices1->push_back((*points)[0]);
	vertices1->push_back((*points)[3]);
	vertices1->push_back((*points)[2]);
	vertices1->push_back((*points)[0]);
	vertices1->push_back((*points)[1]);
	vertices1->push_back((*points)[3]);
	for (int i = 0; i < 6; ++i) normals1->push_back(osg::Vec3(0,-1,0));

	vertices1->push_back((*points)[4]);
	vertices1->push_back((*points)[1]);
	vertices1->push_back((*points)[0]);
	vertices1->push_back((*points)[4]);
	vertices1->push_back((*points)[5]);
	vertices1->push_back((*points)[1]);
	for (int i = 0; i < 6; ++i) normals1->push_back(osg::Vec3(0, 0, -1));

	vertices1->push_back((*points)[6]);
	vertices1->push_back((*points)[5]);
	vertices1->push_back((*points)[4]);
	vertices1->push_back((*points)[6]);
	vertices1->push_back((*points)[7]);
	vertices1->push_back((*points)[5]);
	for (int i = 0; i < 6; ++i) normals1->push_back(osg::Vec3(0, 1, 0));

	vertices1->push_back((*points)[2]);
	vertices1->push_back((*points)[7]);
	vertices1->push_back((*points)[6]);
	vertices1->push_back((*points)[2]);
	vertices1->push_back((*points)[3]);
	vertices1->push_back((*points)[7]);
	for (int i = 0; i < 6; ++i) normals1->push_back(osg::Vec3(0, 0, 1));

	vertices1->push_back((*points)[5]);
	vertices1->push_back((*points)[3]);
	vertices1->push_back((*points)[1]);
	vertices1->push_back((*points)[5]);
	vertices1->push_back((*points)[7]);
	vertices1->push_back((*points)[3]);
	for (int i = 0; i < 6; ++i) normals1->push_back(osg::Vec3(1, 0, 0));

	vertices1->push_back((*points)[6]);
	vertices1->push_back((*points)[0]);
	vertices1->push_back((*points)[2]);
	vertices1->push_back((*points)[6]);
	vertices1->push_back((*points)[4]);
	vertices1->push_back((*points)[0]);
	for (int i = 0; i < 6; ++i) normals1->push_back(osg::Vec3(-1, 0, 0));

	for (int i = 0; i < 6; ++i){
		texcoords1->push_back(osg::Vec2(1, 0));
		texcoords1->push_back(osg::Vec2(0, 1));
		texcoords1->push_back(osg::Vec2(0, 0));

		texcoords1->push_back(osg::Vec2(1, 0));
		texcoords1->push_back(osg::Vec2(0, 1));
		texcoords1->push_back(osg::Vec2(1, 1));
	}

	osg::ref_ptr<osg::Geometry> kocka = new osg::Geometry;
	kocka->setUseVertexBufferObjects(true);

	// állítsuk be, hogy a VBO-ba milyen adatok kerüljenek
	kocka->setVertexArray(vertices1.get());
	kocka->setNormalArray(normals1.get());
	kocka->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	kocka->setTexCoordArray(0, texcoords1.get());
	// kirajzolandó primitív meghatározása
	kocka->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES, 0, 6 * 6));


	// pozíciók
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;

	const int N = 16;
	float delta = 1.0 / N;
	for (int i = 0; i<N; ++i){
		for (int j = 0; j<N; ++j){
			float x = i*delta;
			float y = j*delta;
			// 1. háromszög: x,y x+delta,y y+delta,x
			calc(3, x, y, vertices, normals, texcoords);
			calc(3, x, y + delta, vertices, normals, texcoords);
			calc(3, x + delta, y, vertices, normals, texcoords);
			// 2. háromszög: x+delta,y x+delta,y+delta y+delta,x
			calc(3, x + delta, y, vertices, normals, texcoords);
			calc(3, x, y + delta, vertices, normals, texcoords);
			calc(3, x + delta, y + delta, vertices, normals, texcoords);
		}
	}
	for (int i = 0; i<N; ++i){
		for (int j = 0; j<N; ++j){
			float x = i*delta;
			float y = j*delta;
			// 1. háromszög: x,y x+delta,y y+delta,x
			calc<true>(1, x, y, vertices, normals, texcoords);
			calc<true>(1, x + delta, y, vertices, normals, texcoords);
			calc<true>(1, x, y + delta, vertices, normals, texcoords);
			// 2. háromszög: x+delta,y x+delta,y+delta y+delta,x
			calc<true>(1, x + delta, y, vertices, normals, texcoords);
			calc<true>(1, x + delta, y + delta, vertices, normals, texcoords);
			calc<true>(1, x, y + delta, vertices, normals, texcoords);
		}
	}
	for (int i = 0; i<N; ++i){
		float x = i*delta;
		// 1. háromszög: x,y x+delta,y y+delta,x
		calc2<true>(1, x + delta, 1, vertices, normals, texcoords);
		calc2<true>(3, x, 1, vertices, normals, texcoords);
		calc2<true>(1, x, 1, vertices, normals, texcoords);
		// 2. háromszög: x+delta,y x+delta,y+delta y+delta,x
		calc2<true>(3, x, 1, vertices, normals, texcoords);
		calc2<true>(1, x + delta, 1, vertices, normals, texcoords);
		calc2<true>(3, x + delta, 1, vertices, normals, texcoords);
	}
	for (int i = 0; i<N; ++i){
		float x = i*delta;
		// 1. háromszög: x,y x+delta,y y+delta,x
		calc2<false>(1, x + delta, 0, vertices, normals, texcoords);
		calc2<false>(1, x, 0, vertices, normals, texcoords);
		calc2<false>(3, x, 0, vertices, normals, texcoords);
		// 2. háromszög: x+delta,y x+delta,y+delta y+delta,x
		calc2<false>(3, x, 0, vertices, normals, texcoords);
		calc2<false>(3, x + delta, 0, vertices, normals, texcoords);
		calc2<false>(1, x + delta, 0, vertices, normals, texcoords);
	}

	// négyszög geometria
	osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
	quad->setUseVertexBufferObjects(true);

	// állítsuk be, hogy a VBO-ba milyen adatok kerüljenek
	quad->setVertexArray(vertices.get());
	quad->setNormalArray(normals.get());
	quad->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	quad->setTexCoordArray(0, texcoords.get());
	// kirajzolandó primitív meghatározása
	quad->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES, 0, 2 * 6 * N*N + 2 * 6 * N));

	// textúra betöltése
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile("wood_texture.bmp");
	texture->setImage(image.get());
	texture->setFilter(osg::Texture::FilterParameter::MIN_FILTER, osg::Texture::FilterMode::LINEAR_MIPMAP_LINEAR);
	texture->setFilter(osg::Texture::FilterParameter::MAG_FILTER, osg::Texture::FilterMode::LINEAR);
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::WrapMode::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::WrapMode::REPEAT);
	// textúra betöltése
	osg::ref_ptr<osg::Texture2D> texture2 = new osg::Texture2D;
	osg::ref_ptr<osg::Image> image2 = osgDB::readImageFile("stone_texture.bmp");
	texture2->setImage(image2.get());
	texture2->setFilter(osg::Texture::FilterParameter::MIN_FILTER, osg::Texture::FilterMode::LINEAR_MIPMAP_LINEAR);
	texture2->setFilter(osg::Texture::FilterParameter::MAG_FILTER, osg::Texture::FilterMode::LINEAR);
	texture2->setWrap(osg::Texture::WRAP_S, osg::Texture::WrapMode::REPEAT);
	texture2->setWrap(osg::Texture::WRAP_T, osg::Texture::WrapMode::REPEAT);

	osg::ref_ptr<osg::Geode> koca = new osg::Geode;
	koca->addDrawable(kocka.get());

	osg::ref_ptr<osg::MatrixTransform> oszlop = new osg::MatrixTransform;
	oszlop->setMatrix(osg::Matrix::scale(1, 15, 1)*osg::Matrix::translate(-0.5, 0, -0.5));
	oszlop->addChild(koca.get());

	osg::ref_ptr<osg::Geode> henger = new osg::Geode;
	henger->addDrawable(quad.get());

	// rakjuk be egy geode-ba a quad-ot, mint kirajzolandó elemet!
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(oszlop.get());
	//root->addChild(henger.get());

	for (int i = 0; i < 7; ++i){
		osg::ref_ptr<osg::MatrixTransform> korong = new osg::MatrixTransform;
		float pos = (6 - i) * 1.5;
		float size = i / 5.0 + 1.0;
		korong->setMatrix(osg::Matrix::scale(osg::Vec3(size, 1.0, size))*osg::Matrix::translate(osg::Vec3(0,pos,0)));
		korong->addChild(henger.get());
		root->addChild(korong.get());
	}

	// 0-ás mintavételezõre rakjuk rá a textúrát
	henger->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
	oszlop->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture2.get());
	osg::StateSet* state = root->getOrCreateStateSet();
	state->setMode(GL_LIGHTING,
		osg::StateAttribute::OFF |
		osg::StateAttribute::PROTECTED);
	state->setMode(GL_CULL_FACE, osg::StateAttribute::ON);

	// hozzuk létre a viewer-t és állítsuk be a gyökeret megjelenítendõ adatnak
	osgViewer::Viewer viewer;
	viewer.setSceneData(root.get());

	// a (20,20) kezdeti pozícióba hozzunk létre egy 640x480-as ablakot
	viewer.setUpViewInWindow(20, 20, 640, 480);
	viewer.getCamera()->setClearColor(osg::Vec4(1,1,1,1));
	viewer.realize();

	// írjuk ki egy obj fájlba a parametrikus felületünket!
	osgDB::writeNodeFile(*(henger.get()), "korong.obj");
	osgDB::writeNodeFile(*(oszlop.get()), "oszlop.obj");

	// adjuk át a vezérlést a viewer-nek
	return viewer.run();
}