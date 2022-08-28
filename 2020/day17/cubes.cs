using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace day17
{
    internal abstract class Cubes<T> where T: ITuple
    {
        protected List<T> neighbourMask;
        protected HashSet<T> activeCubes;

        /**
         * <summary> Generate a list of all active cubes and their inactive neighbours
         * and the corresponding count of active neighbours </summary>
         * 
         * <returns> Dictionary of points with corresponding couter values</returns>
         */
        public abstract Dictionary<T, int> FindNumActiveCubeNeighbours();


        public void AddActiveCube(T newCube)
        {
            activeCubes.Add(newCube);
        }
        public void RemoveActiveCube(T cube)
        {
            activeCubes.Remove(cube);
        }

        public HashSet<T> ActiveCubes()
        {
            return activeCubes;
        }

        /**
         * <summary> Updates the active cube lists using the rules of Conways game of life</summary>
         * <param name="activeNeighboursCount"> Dictionary with all coordinates that can change state this round
         * and the corresponding active neighbours count</param>
         */
        public void UpdateActiveCubes(Dictionary<T, int> activeNeighboursCount)
        {
            foreach(var cubeEntry in activeNeighboursCount)
            {
                //If cube is active and less than 2 or more than 3 neighbours are active, cube becomes inactive
                if(activeCubes.Contains(cubeEntry.Key) && !((cubeEntry.Value ==2) || (cubeEntry.Value == 3)))
                {
                    RemoveActiveCube(cubeEntry.Key);
                }

                //If cube is inactive and has 3 neighbours, cube becomes active
                if(!activeCubes.Contains(cubeEntry.Key) && cubeEntry.Value == 3)
                {
                    AddActiveCube(cubeEntry.Key);
                }

                // All other scenarios do not change cubes status

            }
        }

        /**
         * <summary>Simulate Conways game of life. </summary>
         * <param name="rounds"> Number of rounds to simulate.</param>
         */
        public void simulateGameOfLife(int rounds)
        {
            for(int i = 0; i < rounds; i++)
            {
                Dictionary<T, int> activeNeigboursCount = FindNumActiveCubeNeighbours();   
                UpdateActiveCubes(activeNeigboursCount);
            }
        }
        
        public int CountActiveCubes()
        {
            return activeCubes.Count;
        }


    }

    internal class Cubes3D : Cubes<(int x, int y, int z)>
    {
        public Cubes3D(List<string> input)
        {
            //create neighbour Mask
            neighbourMask = Enumerable.Range(-1,3).SelectMany(x=> Enumerable.Range(-1,3).SelectMany(
               y => Enumerable.Range(-1,3).Select(z =>(x,y,z)))).Where(d =>d!=(0,0,0)).ToList();

            //create empty HashSet and populate
            activeCubes = new HashSet<(int x, int y, int z)>();
            for(int i=0; i<input.Count;i++)
            {
                for(int j = 0; j<input[i].Length;j++)
                {
                    if (input[i][j] == '#')
                    {
                        AddActiveCube((i, j, 0));
                    }
                }
            }    
        }

        /**
         * <inheritdoc cref="Cubes<T>.FindNumActiveCubeNeighbours"/>
         */
        public override Dictionary<(int x, int y, int z), int> FindNumActiveCubeNeighbours()
        {
            Dictionary<(int x, int y, int z), int> numActiveNeigbours = new();
            HashSet<(int x, int y, int z)> inactiveNeighbours = new();

            foreach(var cube in activeCubes)
            {
                //find neighbours of each active cube
                numActiveNeigbours.Add(cube, 0);
                foreach(var entry in neighbourMask)
                {
                    (int x, int y, int z) neighbour = (cube.x + entry.x, cube.y + entry.y, cube.z + entry.z);

                    //if neighbour is active, increase count
                    if (activeCubes.TryGetValue(neighbour,out _))
                    {
                        numActiveNeigbours[cube] += 1;
                    }
                    //else neighbour is inactive, add it to inactive pile to be checked later
                    //(if neighbour is already present, silently disregard return value false on the add operation as we do not care)
                    else
                    {
                        inactiveNeighbours.Add(neighbour);
                    }
                }
            }
            
            //Now loop over the inactive neighbours and count their active neighbours
            //All other inactive cubes will have no active neighbours
            foreach(var cube in inactiveNeighbours)
            {
                numActiveNeigbours.Add(cube, 0);
                foreach (var entry in neighbourMask)
                {
                    (int x, int y, int z) neighbour = (cube.x + entry.x, cube.y + entry.y, cube.z + entry.z);

                    //if neighbour is active, increase count; if inactive, no action is required
                    if (activeCubes.TryGetValue(neighbour, out _))
                    {
                        numActiveNeigbours[cube] += 1;
                    }
                }
            }

            return numActiveNeigbours;
        }

    }

    internal class Cubes4D : Cubes<(int x, int y, int z, int w)>
    {
        public Cubes4D(List<string> input)
        {
            //create neighbour Mask
            neighbourMask = Enumerable.Range(-1, 3).SelectMany(x => Enumerable.Range(-1, 3).SelectMany(
               y => Enumerable.Range(-1, 3).SelectMany(z => Enumerable.Range(-1,3).Select(
                   w=> (x, y, z, w))))).Where(d => d != (0, 0, 0, 0)).ToList();

            //create empty HashSet and populate
            activeCubes = new HashSet<(int x, int y, int z, int w)>();
            for (int i = 0; i < input.Count; i++)
            {
                for (int j = 0; j < input[i].Length; j++)
                {
                    if (input[i][j] == '#')
                    {
                        AddActiveCube((i, j, 0, 0));
                    }
                }
            }
        }

        /**
         * <inheritdoc cref="Cubes<T>.FindNumActiveCubeNeighbours"/>
         */
        public override Dictionary<(int x, int y, int z, int w), int> FindNumActiveCubeNeighbours()
        {
            Dictionary<(int x, int y, int z, int w), int> numActiveNeigbours = new();
            HashSet<(int x, int y, int z, int w)> inactiveNeighbours = new();

            foreach (var cube in activeCubes)
            {
                //find neighbours of each active cube
                numActiveNeigbours.Add(cube, 0);
                foreach (var entry in neighbourMask)
                {
                    (int x, int y, int z, int w) neighbour = (cube.x + entry.x, cube.y + entry.y,
                                                              cube.z + entry.z, cube.w + entry.w);

                    //if neighbour is active, increase count
                    if (activeCubes.TryGetValue(neighbour, out _))
                    {
                        numActiveNeigbours[cube] += 1;
                    }
                    //else neighbour is inactive, add it to inactive pile to be checked later
                    //(if neighbour is already present, silently disregard return value false on the add operation as we do not care)
                    else
                    {
                        inactiveNeighbours.Add(neighbour);
                    }
                }
            }

            //Now loop over the inactive neighbours and count their active neighbours
            //All other inactive cubes will have no active neighbours
            foreach (var cube in inactiveNeighbours)
            {
                numActiveNeigbours.Add(cube, 0);
                foreach (var entry in neighbourMask)
                {
                    (int x, int y, int z, int w) neighbour = (cube.x + entry.x, cube.y + entry.y,
                                                              cube.z + entry.z, cube.w + entry.w);

                    //if neighbour is active, increase count; if inactive, no action is required
                    if (activeCubes.TryGetValue(neighbour, out _))
                    {
                        numActiveNeigbours[cube] += 1;
                    }
                }
            }

            return numActiveNeigbours;
        }

    }
}
