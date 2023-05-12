#include "FbxLoader.h"
#include <cassert>


/// <summary>
/// 静的変数の実体	
/// </summary>
const std::string FbxLoader::baseDirectory = "Resources/";


FbxLoader* FbxLoader::GetInstance() {
	static FbxLoader instance;
	return &instance;
}

void FbxLoader::Initialize(ID3D12Device* device) {
	//再初期化チェック
	assert(fbxManager == nullptr);
	//引数からメンバに代入
	this->device = device;
	//マネージャー生成
	fbxManager = FbxManager::Create();
	//マネージャー入出力設定
	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);
	//インポート設定
	fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FbxLoader::Finalize() {
	//各種インスタンスの破棄
	fbxImporter->Destroy();
	fbxManager->Destroy();

}

void FbxLoader::LoadModelFromFile(const string& modelName) {

	//モデルと同じ名前のフォルダから読み込む
	const string directoryPath = baseDirectory + modelName + "/";
	//拡張子の.fbxを付加
	const string fileName = modelName + ".fbx";
	//連結してフルパスを得る
	const string fullPath = directoryPath + fileName;
	//ファイル名を指定してFBXファイルを読み込む	
	if (!fbxImporter->Initialize(fullPath.c_str(), -1, fbxManager->GetIOSettings()))assert(0);

	//シーン生成
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");
	//ファイルからロードしたFBXの情報をシーンにインポート
	fbxImporter->Import(fbxScene);


}
