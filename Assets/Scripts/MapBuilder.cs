using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class MapBuilder : MonoBehaviour {
	private float TileSize = 2.0f;
	private float TileHeight = 3.0f;
	private float HeightMultiplier = 0.2f;

	public void ProcessMap (Map map) {
		CombineInstance[] combineMesh = new CombineInstance[map.Tiles.Length];
		Material[] matarray = new Material[map.Tiles.Length];

		MeshFilter mf = GetComponent<MeshFilter> ();
		mf.mesh = new Mesh ();

		for (int i=0; i<map.Tiles.Length; i++) {
			combineMesh[i].mesh = CreateTile (map,i);
			matarray[i] = new Material(map.TileTypes[map.Tiles[i].TileId].Mat);
		}

		mf.mesh.CombineMeshes (combineMesh, false, false);

		MeshRenderer mr = GetComponent<MeshRenderer> ();
		mr.materials = matarray;

		MeshCollider mc = GetComponent<MeshCollider> ();
		mc.sharedMesh = mf.mesh;
	}

	public Mesh CreateTile (Map map, int tileNumber) {
		Mesh finalMesh = new Mesh();
		List<Vector3> vertices = new List<Vector3> ();
		List<Vector3> normals = new List<Vector3>();
		List<Vector2> uv = new List<Vector2>();
		List<int> triangles = new List<int>();
		List<Color> colours = new List<Color> ();


		bool altTriForm = false;

		int x = tileNumber % map.Width;
		int y = tileNumber / map.Width;

		float height0 = map.HeightBase[tileNumber+map.Width] * HeightMultiplier;
		float height1 = map.HeightBase[tileNumber] * HeightMultiplier;
		float height2 = map.HeightBase[tileNumber+1] * HeightMultiplier;
		float height3 = map.HeightBase[tileNumber+map.Width+1] * HeightMultiplier;
		
		if (map.Tiles [tileNumber].Solid) {
			if ((map.Tiles [tileNumber].Surround & 0x82) == 0x82)
				height0 += TileHeight;
			if ((map.Tiles [tileNumber].Surround & 0x0A) == 0x0A)
				height1 += TileHeight;
			if ((map.Tiles [tileNumber].Surround & 0x28) == 0x28)
				height2 += TileHeight;
			if ((map.Tiles [tileNumber].Surround & 0xA0) == 0xA0)
				height3 += TileHeight;

			if ((map.Tiles [tileNumber].Surround & 0xAA) == 0xAA) {
				uint cornerinfo = map.Tiles [tileNumber].Surround & 0x55;
				if ((cornerinfo & 0x01) == 0x00)
					height0 -= TileHeight;
				if ((cornerinfo & 0x04) == 0x00)
					height1 -= TileHeight;
				if ((cornerinfo & 0x10) == 0x00)
					height2 -= TileHeight;
				if ((cornerinfo & 0x40) == 0x00)
					height3 -= TileHeight;

			}
		}
		
		vertices.Add (new Vector3 (TileSize * x, height0, TileSize * (y + 1)));
		vertices.Add (new Vector3 (TileSize * x, height1, TileSize * y));
		vertices.Add (new Vector3 (TileSize * (x + 1), height2, TileSize * y));
		vertices.Add (new Vector3 (TileSize * (x + 1), height3, TileSize* (y + 1)));
		
		uv.Add (new Vector2 (0.0f, 1.0f));
		uv.Add (new Vector2 (0.0f, 0.0f));
		uv.Add ( new Vector2 (1.0f, 0.0f));
		uv.Add (new Vector2 (1.0f, 1.0f));

		if ((altTriForm == true) ||
		((map.Tiles [tileNumber].Surround & 0x0A) == 0x0A) ||
	    ((map.Tiles[tileNumber].Surround & 0xE0) == 0xE0)) {
			triangles.Add (3);
			triangles.Add (1);
			triangles.Add (0);
			triangles.Add (3);
			triangles.Add (2);
			triangles.Add (1);
		} else {
			triangles.Add (0);
			triangles.Add (2);
			triangles.Add (1);

			triangles.Add (0);
			triangles.Add (3);
			triangles.Add (2);
		}

		finalMesh.normals = normals.ToArray ();
		finalMesh.vertices = vertices.ToArray ();
		finalMesh.triangles = triangles.ToArray();
		finalMesh.uv = uv.ToArray ();

		finalMesh.RecalculateBounds ();
		finalMesh.RecalculateNormals ();

		return finalMesh;
	}
}
