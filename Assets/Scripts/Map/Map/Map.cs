using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.IO;
using System.Xml.Serialization;

public class Map : MonoBehaviour {
	public TileType[] Tiles;
	public List<float> HeightBase;
	public Dictionary<int, TileType> TileTypeDict;
	public Dictionary<int, UnitType> UnitTypeDict;
	public List<Unit> GameUnits;

	public int Width;
	public int Height;

	public void Initialise(Material mat, XmlMap xmlMap) {
		Width = xmlMap.Width;
		Height = xmlMap.Height;

		SetupTileTypes (mat, xmlMap);
		InitialiseTiles (xmlMap);

		tag = "Map";

		// Create the height map
		CreateHeightBase ();
		
		// Create the first surround map
		RecalculateSurround ();
	}
	
	public void SetTile(int tileNumber, int tileType) {
		TileType temp = Tiles [tileNumber];
		Tiles [tileNumber] = (TileType)TileTypeDict [tileType].Clone ();
		
		Tiles [tileNumber].Height = temp.Height;
		Tiles [tileNumber].EC = temp.EC;
		Tiles [tileNumber].ORE = temp.ORE;
	}

	private void SetupTileTypes(Material mat, XmlMap xmlMap) {
		// Make sure the blocks are in the right order
		// TODO why was this necessary?
		Array.Sort (xmlMap.Blocks, (x, y) => x.ID.CompareTo (y.ID));
		
		// A dictionary of the different types of tiles
		TileTypeDict = new Dictionary<int, TileType>();

		foreach (TileType tType in xmlMap.TileTypes) {
			// Read the texture and load it as a texture
			FileStream fs = new FileStream (tType.Tex, FileMode.Open);
			byte[] imageContents = new byte[fs.Length];
			
			fs.Read (imageContents, 0, imageContents.Length);
			fs.Close ();
			
			Texture2D tempTex = new Texture2D(1,1);
			tempTex.LoadImage (imageContents);
			
			// Create a material out of that texture
			tType.Mat = new Material(mat);
			tType.Mat.SetTexture ("_MainTex", tempTex);
			
			TileTypeDict.Add (tType.TileTypeId, tType);
		}

	}

	private void InitialiseTiles(XmlMap xmlMap) {
		// A list of all the tiles on the map
		Tiles = new TileType[xmlMap.Blocks.Length];
		
		// Overwrite the default value from the ones in the xml file
		// TODO there must be a better way of doing this
		for (int i=0; i<xmlMap.Blocks.Length; i++) {
			Tiles[i] = (TileType)TileTypeDict[xmlMap.Blocks[i].TileTypeId].Clone ();
			
			if (xmlMap.Blocks[i].Height > 0)
				Tiles[i].Height = xmlMap.Blocks[i].Height;
			if (xmlMap.Blocks[i].EC >= 0)
				Tiles[i].EC = xmlMap.Blocks[i].EC;
			if (xmlMap.Blocks[i].ORE >= 0)
				Tiles[i].ORE = xmlMap.Blocks[i].ORE;
			
		}
	}

	private void CreateHeightBase() {
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
	
	public void RecalculateSurround() {
		//   1|..|64 
		//  --------
		//   2| X|32
		//  --------
		//   4| 8|16
		
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
				SetTile (i, 0);
				RecalculateSurround ();
			}
			
			Tiles[i].Surround = (uint)surround;
		}

		// Recalculate the A* nodes
		GameObject unitGameobject = GameObject.Find ("Units");
		NewAstar aStar = unitGameobject.GetComponent<NewAstar> ();

		// But only if the A* controller has been initialised
		if (aStar.Initialised)
			aStar.CalculateGraph ();
	}
}