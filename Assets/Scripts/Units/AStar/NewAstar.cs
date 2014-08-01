using UnityEngine;
using System.Collections;

using EMK.Cartography;
using System.Collections.Generic;

public class NewAstar : MonoBehaviour {
	public bool Initialised = false;

	private List<Node> AStarNodes;
	private Graph G;

	public void Initialise () {
		// Get the map component
		GameObject mapGameObject = GameObject.Find ("Map");
		Map map = mapGameObject.GetComponent<Map> ();

		G = new Graph ();

		AStarNodes = new List<Node> ();

		// Add the nodes of the graph
		for (int y=0; y<=map.Height; y++) {
			for (int i=0; i<2; i++) {
				AStarNodes.Add (new Node(0.0, y*4.0+i*2, 0));
				G.AddNode (AStarNodes[AStarNodes.Count - 1]);

				for (int x=0; x<map.Width; x++) {
					AStarNodes.Add (new Node(x*4.0+2.0, y*4.0+i*2.0, 0));
					G.AddNode (AStarNodes[AStarNodes.Count - 1]);

					AStarNodes.Add (new Node(x*4.0+4.0, y*4.0+i*2.0, 0));
					G.AddNode (AStarNodes[AStarNodes.Count - 1]);
				}
			}
		}

		// Add the 'top layer' of nodes to the graph
		for (int i=0; i<2*map.Width+1; i++) {
			AStarNodes.Add (new Node(i*2.0, (map.Height+1)*4.0, 0));
			G.AddNode (AStarNodes[AStarNodes.Count-1]);
		}

		// Do some smeksy maths to calculate the arcs of the graph
		int step = 2*map.Width+1;

		for (int i=0; i<map.Tiles.Length; i++) {
			int y = i/map.Width;
			int x = i%map.Width;

			int base_ = 2*y*step + 2*x;

			// Only new arcs are added (the graph will always be simple)
			G.Add2Arcs (AStarNodes[base_], AStarNodes[base_+1], 1.0f);
			G.Add2Arcs (AStarNodes[base_+1], AStarNodes[base_+2], 1.0f);
			G.Add2Arcs (AStarNodes[base_], AStarNodes[base_+step], 1.0f);
			G.Add2Arcs (AStarNodes[base_+1], AStarNodes[base_+step+1], 1.0f);
			G.Add2Arcs (AStarNodes[base_+2], AStarNodes[base_+step+2], 1.0f);
			G.Add2Arcs (AStarNodes[base_+step], AStarNodes[base_+step+1], 1.0f);
			G.Add2Arcs (AStarNodes[base_+step+1], AStarNodes[base_+step+2], 1.0f);
			G.Add2Arcs (AStarNodes[base_+step], AStarNodes[base_+2*step], 1.0f);
			G.Add2Arcs (AStarNodes[base_+step+1], AStarNodes[base_+2*step+1], 1.0f);
			G.Add2Arcs (AStarNodes[base_+step+2], AStarNodes[base_+2*step+2], 1.0f);
			G.Add2Arcs (AStarNodes[base_+2*step], AStarNodes[base_+2*step+1], 1.0f);
			G.Add2Arcs (AStarNodes[base_+2*step+1], AStarNodes[base_+2*step+2], 1.0f);
		}

		Initialised = true;
	}

	// Determine which nodes can be passed through
	public void CalculateGraph() {
		GameObject mapGameObject = GameObject.Find ("Map");
		Map map = mapGameObject.GetComponent<Map> ();

		Debug.Log ("Start");

		// Set all the nodes to be passable
		// The ones which are impassable will be calculated in a sec...
		foreach (Node n in AStarNodes) {
			n.Passable = true;
		}

		// For each tile on the map
		int step = 2*map.Width+1;
		for (int i=0; i<map.Tiles.Length; i++) {
			// If the tile is solid (i.e. impassable) set the nodes
			// 'under' the tile to be impassable
			if (map.Tiles[i].Solid) {
				int y = i/map.Width;
				int x = i%map.Width;

				// More smeksy maths
				int base_ = 2*y*step + 2*x;

				AStarNodes[base_].Passable = false;
				AStarNodes[base_+1].Passable = false;
				AStarNodes[base_+2].Passable = false;
				AStarNodes[base_+step].Passable = false;
				AStarNodes[base_+step+1].Passable = false;
				AStarNodes[base_+step+2].Passable = false;
				AStarNodes[base_+2*step].Passable = false;
				AStarNodes[base_+2*step+1].Passable = false;
				AStarNodes[base_+2*step+2].Passable = false;
			}
		}

		// Update the actual nodes
		for (int i=0; i<map.Tiles.Length; i++) {
			G.Nodes[i] = AStarNodes[i];
		}
		Debug.Log ("End");
	}

	// Calculate a path from start to end
	// start and end should be in world space
	public Arc[] Calculate(Vector3 start, Vector3 end) {
		double dist;

		Node startNode = G.ClosestNode(start.x, start.z, 0, out dist, true);
		Node endNode = G.ClosestNode(end.x, end.z, 0, out dist, true);

		AStar AS = new AStar (G);

		// If a path is found, return it
		if (AS.SearchPath (startNode, endNode)) {
			return AS.PathByArcs;
		}

		// Otherwise return an empty array
		Arc[] toreturn = new Arc[0];
		return toreturn;
	}
}