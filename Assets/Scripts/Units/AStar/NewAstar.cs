using UnityEngine;
using System.Collections;

using EMK.Cartography;
using System.Collections.Generic;

public class NewAstar : MonoBehaviour {
	public bool Initialised = false;

	private List<Node> AStarNodes;
	private Graph G;

	public void Initialise () {
		GameObject mapGameObject = GameObject.Find ("Map");
		Map map = mapGameObject.GetComponent<Map> ();

		G = new Graph ();

		AStarNodes = new List<Node> ();

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

		for (int i=0; i<2*map.Width+1; i++) {
			AStarNodes.Add (new Node(i*2.0, (map.Height+1)*4.0, 0));
			G.AddNode (AStarNodes[AStarNodes.Count-1]);
		}

		int step = 2*map.Width+1;
		for (int i=0; i<map.Tiles.Length; i++) {
			int y = i/map.Width;
			int x = i%map.Width;

			int base_ = 2*y*step + 2*x;

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

	public void CalculateGraph() {
		GameObject mapGameObject = GameObject.Find ("Map");
		Map map = mapGameObject.GetComponent<Map> ();

		foreach (Node n in AStarNodes) {
			n.Passable = true;
		}

		int step = 2*map.Width+1;
		for (int i=0; i<map.Tiles.Length; i++) {
			if (map.Tiles[i].Solid) {
				int y = i/map.Width;
				int x = i%map.Width;
				
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

		for (int i=0; i<map.Tiles.Length; i++) {
			G.Nodes[i] = AStarNodes[i];
		}
	}

	public Arc[] Calculate(Vector3 start, Vector3 end) {
		double dist;

		Node startNode = G.ClosestNode(start.x, start.z, 0, out dist, true);
		Node endNode = G.ClosestNode(end.x, end.z, 0, out dist, true);

		AStar AS = new AStar (G);

		if (AS.SearchPath (startNode, endNode)) {
			return AS.PathByArcs;
		}
			
		Arc[] toreturn = new Arc[0];
		return toreturn;
	}
}