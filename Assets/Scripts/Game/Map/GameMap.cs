using UnityEngine;
using System.Collections;
using RR2.Statics;
using System.Collections.Generic;
using System.Reflection;
using System;
using System.IO;

namespace RR2.Map {
	[RequireComponent(typeof(MeshFilter))]
	[RequireComponent(typeof(MeshRenderer))]
	[RequireComponent(typeof(MeshCollider))]
	public class GameMap : MonoBehaviour {

		public Material DefaultMaterial;

		public Dictionary<int, GameTileType> GameTileTypes;
		public GameTileType[] Tiles;

		public Dictionary<int, GamePathType> GamePathTypes;

		public int Width;
		public int Height;

		public List<float> HeightBase;
		private float TileHeight = 4.0f;
		public float TileSize = 4.0f;

		// Use this for initialization
		void Awake () {
			//MapData mapData = ApplicationModel.Instance.MapData.MapData;

			TileType[] mapTileTypes = ApplicationModel.Instance.MapData.TileTypes;
			PathType[] mapPathTypes = ApplicationModel.Instance.MapData.PathTypes;
			Tile[] mapTiles = ApplicationModel.Instance.MapData.MapData.Tiles;

			Width = ApplicationModel.Instance.MapData.MapData.Width;
			Height = ApplicationModel.Instance.MapData.MapData.Height;

			Tiles = new GameTileType[Width * Height];

			GameTileTypes = new Dictionary<int, GameTileType> ();

			foreach (TileType tileType in mapTileTypes) {
				GameTileType toAdd = new GameTileType(tileType);

				toAdd.Texture = LoadTexture(toAdd.TexturePath);

				GameTileTypes.Add(toAdd.TileTypeId, toAdd);
			}

			GamePathTypes = new Dictionary<int, GamePathType> ();
			foreach (PathType pathType in mapPathTypes) {
				GamePathType toAdd = new GamePathType(pathType);

				toAdd.Texture = LoadTexture(toAdd.TexturePath);

				GamePathTypes.Add(toAdd.PathTypeId, toAdd);
			}

			foreach(Tile tile in mapTiles) {
				GameTileType tempType = (GameTileType)GameTileTypes[tile.TileTypeId].Clone();

				tempType.InheritFromTile(tile);

				Tiles[tile.TileId] = tempType;
			}

			CreateHeightBase ();
			RecalculateSurround ();

			ProcessMap ();
			
			//ApplicationModel.Instance.Check ();
		}

		public void SetTile(int tileNumber, int tileType) {
			Debug.Log (string.Format ("Setting tile {0} to type {1}", tileNumber, tileType));

			GameTileType temp = Tiles [tileNumber];
			Tiles [tileNumber] = (GameTileType)GameTileTypes [tileType].Clone ();

			// TODO this isn't right
			// Later note - what isn't right?
			// Even later note - wow, such commenting
			Tiles [tileNumber].Height = temp.Height;

			RecalculateSurround ();
			ProcessMap ();

		}

		public void RecalculateSurround() {
			//   1|..|64 
			//  --------
			//   2| X|32
			//  --------
			//   4| 8|16

			Debug.Log ("Recalculating surround");

			// For each tile, work out which tiles are present.
			// Each bit corresponds to a surrounding point, as shown in diagram above.
			// A calculator or something may be needed to work this out :P
			for (int i = 0; i<Tiles.Length; i++) {
				byte surround = 0;
				
				if (i % Width == Width - 1) // On the right hand side of the map
					surround |= 0x70;
				else if (i<Tiles.Length - 1) { // A solid can be to the right (not at the edge)
					if (Tiles[i+1].Solid) // There is a solid on the right
						surround |= 0x20;
				}
				
				if (i<Tiles.Length - Width - 1) { // If top right exists
					if (Tiles[i+Width+1].Solid) // If top right solid
						surround |= 0x40;
				}
				
				if (i<Tiles.Length - Width) { // If top left exists
					if (Tiles[i+Width-1].Solid) // If top left solid
						surround |= 0x01;
				}
				
				if (i % Width == 0) // On the left hand side of the map
					surround |= 0x07;
				else if (i>0) { // A solid can be on the left
					if (Tiles[i-1].Solid) // A solid is on the left
						surround |= 0x02;
				}
				
				
				if (i>=Width) { // If solid can be on the bottom right
					if (Tiles[i-Width+1].Solid) // If bottom right solid
						surround |= 0x10;
				}
				
				if (i> Width + 1) { // If bottom left exists
					if (Tiles[i-Width-1].Solid) { // If bottom left solid
						surround |= 0x04;
					}
				}
				
				
				if (i / Width == 0) // On the bottom of the map
					surround |= 0x1C;
				else if (i >= Width) { // Not on the bottom row
					if (Tiles[i-Width].Solid) // If there is a solid below 
						surround |= 0x08;
				}
				
				if (i / Width == Height - 1) // On the top of the map
					surround |= 0xC1;
				else if (i < Tiles.Length - Width) { // Not on the top row
					if (Tiles[i+Width].Solid) // If solid tile in row above
						surround |= 0x80;
				}
				
				// If the tile's surround is 'invalid' (cannot exist), replace the
				// tile with tile type 0.
				if (Tiles[i].Solid &&
				    (((surround & 0xAA) == 0x88) ||
				 ((surround & 0xAA) == 0x22) ||
				 ((surround & 0xAA) == 0x02) ||
				 ((surround & 0xAA) == 0x08) ||
				 ((surround & 0xAA) == 0x20) ||
				 ((surround & 0xAA) == 0x80) ||
				 ((surround & 0xAA) == 0x00)))
				{
					Debug.Log (string.Format("Invalid tile at {0}", i));
					//SetTile (i, 0);
					MapController.Instance.DrillTile(i);
					//RecalculateSurround ();
				}
				
				Tiles[i].Surround = (uint)surround;
			}
		}

		public void ProcessMap () {
			Debug.Log ("Processing Map");
			
			// Used to create the final map
			CombineInstance[] combineMesh = new CombineInstance[Tiles.Length];
			Material[] matarray = new Material[Tiles.Length];
			
			MeshFilter mf = GetComponent<MeshFilter> ();
			mf.mesh = new Mesh ();
			
			// For each tile, create the relevant mesh
			for (int i=0; i<Tiles.Length; i++) {
				combineMesh[i].mesh = CreateTile (i);
				
				Material toadd = new Material(DefaultMaterial);
				toadd.SetTexture ("_MainTex", Tiles[i].Texture);
				// TODO tidy this up
				if (Tiles[i].PathTypeId > 0) {
					toadd.SetTexture ("_PathTex", GamePathTypes[Tiles[i].PathTypeId].Texture);
				}
				matarray[i] = toadd;
			}
			
			// Combine the meshes
			mf.mesh.CombineMeshes (combineMesh, false, false);
			
			// Add the mesh to the renderer and collider
			MeshRenderer mr = GetComponent<MeshRenderer> ();
			mr.materials = matarray;
			MeshCollider mc = GetComponent<MeshCollider> ();
			mc.sharedMesh = mf.mesh;
		}
		
		// Cretaes the relevant tile mesh
		public Mesh CreateTile (int tileNumber) {
			//Debug.Log (string.Format ("Creating tile {0}", tileNumber));
			
			Mesh finalMesh = new Mesh();
			List<UnityEngine.Vector3> vertices = new List<UnityEngine.Vector3> ();
			//List<UnityEngine.Vector3> normals = new List<UnityEngine.Vector3>();
			List<Vector2> uv = new List<Vector2>();
			List<int> triangles = new List<int>();
			//List<Color> colours = new List<Color> ();
			
			// If the tile's triangles should be arranged 'the other way round'
			bool altTriForm = false;
			
			// Calculate the position of the tile (used later)
			int x = tileNumber % Width;
			int y = tileNumber / Width;
			
			// Assign a height to each corner of the tile
			float height0 = HeightBase [tileNumber + Width];// * HeightMultiplier;
			float height1 = HeightBase[tileNumber];// * HeightMultiplier;
			float height2 = HeightBase[tileNumber+1];// * HeightMultiplier;
			float height3 = HeightBase[tileNumber+Width+1];// * HeightMultiplier;
			
			// If the tile is solid, work out which corners should be 'high'
			if (Tiles [tileNumber].Solid) {
				if ((Tiles [tileNumber].Surround & 0x82) == 0x82)
					height0 += TileHeight;
				if ((Tiles [tileNumber].Surround & 0x0A) == 0x0A)
					height1 += TileHeight;
				if ((Tiles [tileNumber].Surround & 0x28) == 0x28)
					height2 += TileHeight;
				if ((Tiles [tileNumber].Surround & 0xA0) == 0xA0)
					height3 += TileHeight;
				
				if ((Tiles [tileNumber].Surround & 0xAA) == 0xAA) {
					// TODO is this needed?
					uint cornerinfo = Tiles [tileNumber].Surround & 0x55;
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
			vertices.Add (new UnityEngine.Vector3 (TileSize * x, height0, TileSize * (y + 1)));
			vertices.Add (new UnityEngine.Vector3 (TileSize * x, height1, TileSize * y));
			vertices.Add (new UnityEngine.Vector3 (TileSize * (x + 1), height2, TileSize * y));
			vertices.Add (new UnityEngine.Vector3 (TileSize * (x + 1), height3, TileSize* (y + 1)));
			
			// TODO sort out UV for tiles
			uv.Add (new Vector2 (0.0f, 1.0f));
			uv.Add (new Vector2 (0.0f, 0.0f));
			uv.Add ( new Vector2 (1.0f, 0.0f));
			uv.Add (new Vector2 (1.0f, 1.0f));
			
			// Work out where the triangles should be (??)
			if ((altTriForm == true) ||
			    ((Tiles [tileNumber].Surround & 0x0A) == 0x0A) ||
			    ((Tiles[tileNumber].Surround & 0xE0) == 0xE0)) {
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

		private void CreateHeightBase() {
			Debug.Log ("Creating Height Base");

			// The list of the heightmap, one value for each corner-point on the map.
			// These corner points are shared.
			HeightBase = new List<float> ();
			
			// The height of the 'bottom' corners of the bottom row have no
			// calculatable height value. Just use the tile's height value.
			// Also, the bottom-left corner of the first tile on the second row's
			// height is not know. Also just use tile's height value.
			for (int i = 0; i<=Width; i++) {
				HeightBase.Add (Tiles[i].Height);
			}
			
			// For each tile work out the height of the top right corner.
			// This includes the bottom row.
			for (int i=0; i<Height-1; i++) {
				HeightBase.Add (Tiles [Width * i].Height);
				for (int j=0; j<Width-1; j++) {
					// The average of the four surrounding tiles
					float height = (float)(Tiles [Width * i + j].Height +
					                       Tiles [Width * i + j + 1].Height +
					                       Tiles [Width * i + j + Width].Height +
					                       Tiles [Width * i + j + 1 + Width].Height) / 4;
					HeightBase.Add (height);
				}
			}
			
			// For the 'top' corners of the top row, also use height value.
			for (int i = Width; i>0; i--) {
				HeightBase.Add (Tiles[Tiles.Length - i].Height);
			}
		}

		private Texture2D LoadTexture (string dir) {
			Debug.Log (string.Format ("Loading texture {0}", dir));
			// Read the texture and load it as a texture
			FileStream fs = new FileStream (dir, FileMode.Open);
			byte[] imageContents = new byte[fs.Length];
			
			fs.Read (imageContents, 0, imageContents.Length);
			fs.Close ();
			
			Texture2D tempTex = new Texture2D(1,1);
			tempTex.LoadImage (imageContents);
			return tempTex;
		}
	}

	public class GamePathType : PathType, ICloneable {
		public Texture2D Texture;

		public GamePathType(PathType c) : base()
		{
			// copy base class properties.
			foreach (PropertyInfo prop in c.GetType().GetProperties())
			{
				PropertyInfo prop2 = c.GetType().GetProperty(prop.Name);
				prop2.SetValue(this, prop.GetValue(c, null), null);
			}
		}

		public object Clone() {
			return this.MemberwiseClone ();
		}
	}

	public class GameTileType : TileType, ICloneable {
		public Texture2D Texture;
		public Material Material;// = Material.mainTexture;
		public UInt32 Surround;
		public bool Selected = false;

		public GameTileType(TileType c) : base()
		{
			// copy base class properties.
			foreach (PropertyInfo prop in c.GetType().GetProperties())
			{
				PropertyInfo prop2 = c.GetType().GetProperty(prop.Name);
				prop2.SetValue(this, prop.GetValue(c, null), null);
			}
		}

		public void InheritFromTile(Tile t) {
			foreach (PropertyInfo property in t.GetType().GetProperties( BindingFlags.Public | BindingFlags.Instance))
			{
				PropertyInfo targetProperty = this.GetType().GetProperty( property.Name, BindingFlags.Public | BindingFlags.Instance );
				if (targetProperty != null
				    && targetProperty.CanWrite)
				{
					targetProperty.SetValue( this, property.GetValue(t, null), null );
				}
			}
		}

		public object Clone() {
			return this.MemberwiseClone ();
		}
	}
}
