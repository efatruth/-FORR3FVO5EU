//-----------------------------------------------------------------------
// <copyright file="AugmentedImageExampleController.cs" company="Google">
//
// Copyright 2018 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// </copyright>
//-----------------------------------------------------------------------

namespace GoogleARCore.Examples.AugmentedImage
{
    using System.Collections.Generic;
    using System.Runtime.InteropServices;
    using GoogleARCore;
    using UnityEngine;
    using UnityEngine.UI;

    /// <summary>
    /// Controller for AugmentedImage example.
    /// </summary>
    public class AugmentedImageExampleController : MonoBehaviour
    {
        /// <summary>
        /// A prefab for visualizing an AugmentedImage.
        /// </summary>
        public AugmentedImageVisualizer AugmentedImageVisualizerPrefab;

        /// <summary>
        /// The overlay containing the fit to scan user guide.
        /// </summary>
        public GameObject FitToScanOverlay;

        private Dictionary<int, AugmentedImageVisualizer> m_Visualizers
            = new Dictionary<int, AugmentedImageVisualizer>();

        private List<AugmentedImage> m_TempAugmentedImages = new List<AugmentedImage>();

        /// <summary>
        /// The Unity Update method.
        /// </summary>
        public void Update()
        {
            // Exit the app when the 'back' button is pressed.
            if (Input.GetKey(KeyCode.Escape))
            {
                Application.Quit();
            }

            // Checkar hvort það sé kveikt á motion tracking
            if (Session.Status != SessionStatus.Tracking)
            {
                return;
            }

            // Fær allar myndir sem er hægt að nota
            Session.GetTrackables<AugmentedImage>(m_TempAugmentedImages, TrackableQueryFilter.Updated);

            // Býr til visualizer fyrir myndir sem var verið að finna og tekur þær sem eru komnar út úr frameinu
            foreach (var image in m_TempAugmentedImages)
            {
                AugmentedImageVisualizer visualizer = null;
                m_Visualizers.TryGetValue(image.DatabaseIndex, out visualizer); // Checkar hvort að myndi sé nú þegar inn í gagnagrunninum og með visualizer
                if (image.TrackingState == TrackingState.Tracking && visualizer == null) // Ef það er ekki og ef það er verið að tracka myndina
                {
                    Anchor anchor = image.CreateAnchor(image.CenterPose); // Býr til anchor fyrir hana (sem ARCore notar til að halda utan um staðsetninguna)
                    visualizer = (AugmentedImageVisualizer)Instantiate(AugmentedImageVisualizerPrefab, anchor.transform); // Býr til nýan visualizer sem gameobject
                    visualizer.Image = image; // Setur myndina á visualizernum í það sem hún á að vera
                    m_Visualizers.Add(image.DatabaseIndex, visualizer); // Bætir visualizernum í gagnagrunnin
                }
                else if (image.TrackingState == TrackingState.Stopped && visualizer != null) // Ef það er en myndin er ekki lengur inn í frameinu
                {
                    m_Visualizers.Remove(image.DatabaseIndex); // Tekur hana úr gagnagrunninum
                    GameObject.Destroy(visualizer.gameObject); // Eyðir geymobjectinu fyrir visualizerinn
                }
                // Annars ef hún er til og er ennþá enni í frameinu þarf ekki að gera neitt
            }

            // Sýnir ui fyrir þegar engin mynd finnst
            foreach (var visualizer in m_Visualizers.Values)
            {
                if (visualizer.Image.TrackingState == TrackingState.Tracking)
                {
                    FitToScanOverlay.SetActive(false);
                    return;
                }
            }

            FitToScanOverlay.SetActive(true);
        }
    }
}
