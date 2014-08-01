using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class MapBuilder : MonoBehaviour {
	// These are quite imporatant, and shouldn't be able to be changed
	private float TileSize = 4.0f;
	private float TileHeight = 5.0f;
	private float HeightMultiplier = 0.2f;

	public void ProcessMap () {
		Map map = gameObject.GetComponent<Map> ();

		// Used to create the final map
		CombineInstance[] combineMesh = new CombineInstance[map.Tiles.Length];
		Material[] matarray = new Material[map.Tiles.Length];

		MeshFilter mf = GetComponent<MeshFilter> ();
		mf.mesh = new Mesh ();

		// For each tile, create the relevant mesh
		for (int i=0; i<map.Tiles.Length; i++) {
			combineMesh[i].mesh = CreateTile (i);

			Material toadd = new Material(map.TileTypeDict[map.Tiles[i].TileTypeId].Mat);
			if (map.Tiles[i].PathTypeId > 0) {
				toadd.SetTexture ("_PathTex", map.PathTypeDict[map.Tiles[i].PathTypeId].LoadedTex);
			}
			//Material toadd = map.TileTypeDict[map.Tiles[i].TileTypeId].Mat;
			//toadd.SetTexture ("_PathTex", map.PathTypeDict[1].LoadedTex);

			//matarray[i] = new Material(map.TileTypeDict[map.Tiles[i].TileTypeId].Mat);
			matarray[i] = toadd;
		}

		// Combine the meshes
		mf.mesh.CombineMeshes (combineMesh, false, false);

		// Add the mesh to the renderer and collider
		MeshRenderer mr = GetComponent<MeshRenderer> ();
		mr.materials = matarray;
		MeshCollider mc = GetComponent<MeshCollider> ();
		mc.sharedMesh = mf.mesh;

		GameObject cam = GameObject.Find ("Main Camera");
		MyStratCamera msc = cam.GetComponent<MyStratCamera> ();
		msc.PanLimits = new Vector3 (map.Width * TileSize, 0, map.Height * TileSize);


		// Recalculate the A* nodes
		NewAstar aStar = GameObject.Find ("Units").GetComponent<NewAstar> ();
		
		// But only if the A* controller has been initialised
		if (aStar.Initialised)
			aStar.CalculateGraph ();

	}

	// Cretaes the relevant tile mesh
	public Mesh CreateTile (int tileNumber) {
		Map map = gameObject.GetComponent<Map> ();

		Mesh finalMesh = new Mesh();
		List<Vector3> vertices = new List<Vector3> ();
		List<Vector3> normals = new List<Vector3>();
		List<Vector2> uv = new List<Vector2>();
		List<int> triangles = new List<int>();
		List<Color> colours = new List<Color> ();

		// If the tile's triangles should be arranged 'the other way round'
		bool altTriForm = false;

		// Calculate the position of the tile (used later)
		int x = tileNumber % map.Width;
		int y = tileNumber / map.Width;

		// Assign a height to each corner of the tile
		float height0 = map.HeightBase[tileNumber+map.Width] * HeightMultiplier;
		float height1 = map.HeightBase[tileNumber] * HeightMultiplier;
		float height2 = map.HeightBase[tileNumber+1] * HeightMultiplier;
		float height3 = map.HeightBase[tileNumber+map.Width+1] * HeightMultiplier;

		// If the tile is solid, work out which corners should be 'high'
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
				// TODO is this needed?
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

		// Assign the vertices
		vertices.Add (new Vector3 (TileSize * x, height0, TileSize * (y + 1)));
		vertices.Add (new Vector3 (TileSize * x, height1, TileSize * y));
		vertices.Add (new Vector3 (TileSize * (x + 1), height2, TileSize * y));
		vertices.Add (new Vector3 (TileSize * (x + 1), height3, TileSize* (y + 1)));

		// TODO sort out UV for tiles
		uv.Add (new Vector2 (0.0f, 1.0f));
		uv.Add (new Vector2 (0.0f, 0.0f));
		uv.Add ( new Vector2 (1.0f, 0.0f));
		uv.Add (new Vector2 (1.0f, 1.0f));

		// Work out where the triangles should be (??)
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

		// Not needed
		// finalMesh.normals = normals.ToArray ();
		// TODO smooth normals

		// Assign the required data
		finalMesh.vertices = vertices.ToArray ();
		finalMesh.triangles = triangles.ToArray();
		finalMesh.uv = uv.ToArray ();

		// Recalculate stuff
		finalMesh.RecalculateBounds ();
		finalMesh.RecalculateNormals ();

		return finalMesh;
	}
}
