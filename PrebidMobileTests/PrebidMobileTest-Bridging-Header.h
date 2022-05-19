/*   Copyright 2018-2021 Prebid.org, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#import "MockServer.h"
#import "MockServerURLProtocol.h"
#import "MockServerRule.h"
#import "MockServerRuleRedirect.h"
#import "MockServerRuleSlow.h"
#import "MockServerMimeType.h"

//Imports
#import "PBMAbstractCreative.h"
#import "PBMAdDetails.h"
#import "PBMAdLoadManagerBase.h"
#import "PBMAdLoadManagerProtocol.h"
#import "PBMAdLoadManagerVAST.h"
#import "PBMAdLoadManagerDelegate.h"
#import "PBMAdRefreshOptions.h"
#import "PBMAdViewManager.h"
#import "PBMAdViewManagerDelegate.h"
#import "PBMAppInfoParameterBuilder.h"
#import "PBMBasicParameterBuilder.h"
#import "PBMClickthroughBrowserView.h"
#import "PBMClickthroughBrowserView+NavigationHandler.h"
#import "PBMClickthroughBrowserViewDelegate.h"
#import "PBMConstants.h"
#import "PBMCreativeFactory.h"
#import "PBMCreativeFactoryJob.h"
#import "PBMCreativeModel.h"
#import "PBMCreativeModelCollectionMakerVAST.h"
#import "PBMCreativeResolutionDelegate.h"
#import "PBMCreativeViewDelegate.h"
#import "PBMDeepLinkPlus.h"
#import "PBMDeepLinkPlusHelper.h"
#import "PBMDeepLinkPlusHelper+Testing.h"
#import "PBMDeviceAccessManager.h"
#import "PBMDeviceAccessManagerKeys.h"
#import "PBMDeviceInfoParameterBuilder.h"
#import "PBMDownloadDataHelper.h"
#import "PBMError.h"
#import "PBMAdModelEventTracker.h"
#import "PBMFunctions.h"
#import "PBMFunctions+Private.h"
#import "PBMFunctions+Testing.h"
#import "PBMGeoLocationParameterBuilder.h"
#import "PBMHTMLCreative.h"
#import "PBMHTMLFormatter.h"
#import "PBMInterstitialDisplayProperties.h"
#import "PBMJSLibrary.h"
#import "PBMJSLibraryManager.h"
#import "PBMLocationManager.h"
#import "PBMMacros.h"
#import "PBMModalManager.h"
#import "PBMModalState.h"
#import "PBMModalViewController+Private.h"
#import "PBMModalViewControllerDelegate.h"
#import "PBMNonModalViewController.h"
#import "PBMModalAnimator.h"
#import "PBMModalPresentationController.h"
#import "PBMTouchForwardingView.h"
#import "PBMMRAIDCommand.h"
#import "PBMMRAIDConstants.h"
#import "PBMMRAIDController.h"
#import "PBMMRAIDJavascriptCommands.h"
#import "PBMNetworkParameterBuilder.h"
#import "PBMNSThreadProtocol.h"
#import "PBMORTB.h"
#import "PBMORTBParameterBuilder.h"
#import "PBMParameterBuilderProtocol.h"
#import "PBMParameterBuilderService.h"
#import "PBMReachability.h"
#import "PBMSupportedProtocolsParameterBuilder.h"
#import "PBMTrackingEvent.h"
#import "PBMTrackingRecord.h"
#import "PBMTransaction.h"
#import "PBMUIApplicationProtocol.h"
#import "PBMURLComponents.h"
#import "PBMUserAgentService.h"
#import "PBMUserConsentDataManager.h"
#import "PBMUserConsentResolver.h"
#import "PBMUserConsentParameterBuilder.h"
#import "PBMDeviceAccessManagerKeys.h"
#import "PBMAdRequesterVAST.h"
#import "PBMCreativeModelCollectionMakerVAST.h"
#import "PBMTransaction.h"
#import "PBMVideoCreative.h"
#import "PBMVideoView.h"
#import "PBMVideoViewDelegate.h"
#import "PBMWebView.h"
#import "PBMWebViewDelegate.h"
#import "PBMAdRequestResponseVAST.h"
#import "PBMCircularProgressBarLayer.h"
#import "PBMInterstitialLayoutConfigurator.h"
#import "PBMCreativeViewabilityTracker.h"
#import "PBMSKAdNetworksParameterBuilder.h"

// Extensions
#import "NSException+PBMExtensions.h"
#import "NSString+PBMExtensions.h"
#import "PBMTouchDownRecognizer.h"
#import "UIView+PBMExtensions.h"
#import "UIWindow+PBMExtensions.h"
#import "WKNavigationAction+PBMWKNavigationActionCompatible.h"
#import "WKWebView+PBMWKWebViewCompatible.h"

// VAST
#import "PBMVastAbstractAd.h"
#import "PBMVastAdsBuilder.h"
#import "PBMVastCreativeAbstract.h"
#import "PBMVastCreativeCompanionAds.h"
#import "PBMVastCreativeCompanionAdsCompanion.h"
#import "PBMVastCreativeLinear.h"
#import "PBMVastCreativeNonLinearAds.h"
#import "PBMVastCreativeNonLinearAdsNonLinear.h"
#import "PBMVastGlobals.h"
#import "PBMVastIcon.h"
#import "PBMVastInlineAd.h"
#import "PBMVastMediaFile.h"
#import "PBMVastParser+Private.h"
#import "PBMVastResourceContainerProtocol.h"
#import "PBMVastResponse.h"
#import "PBMVastTrackingEvents.h"
#import "PBMVastWrapperAd.h"

// 3dPartyWrappers
#import "PBMOpenMeasurementSession.h"
#import "PBMOpenMeasurementWrapper.h"
#import "PBMOpenMeasurementEventTracker.h"
#import "PBMOpenMeasurementFriendlyObstructionTypeBridge.h"

// Tests
#import "PBMTransaction+PBMTestExtension.h"
#import "PBMCreativeFactoryJob+PBMTestExtension.h"
#import "PBMAbstractCreative+PBMTestExtension.h"
#import "PBMAdLoadManager+PBMTestExtension.h"
#import "PBMAdViewManager+PBMTestExtension.h"
#import "PBMAutoRefreshManager+PBMTestExtension.h"
#import "PBMCreativeFactoryJob+PBMTestExtension.h"
#import "PBMHTMLCreative+PBMTestExtension.h"
#import "PBMLocationManager+PBMTestExtension.h"
#import "PBMModalManager+PBMTestExtension.h"
#import "PBMModalViewController+PBMTestExtension.h"
#import "PBMOpenMeasurementWrapper+PBMTestExtension.h"
#import "PBMOpenMeasurementSession+PBMTestExtension.h"
#import "PBMAdLoadManager+PBMTestExtension.h"
#import "PBMUserAgentService+PBMTestExtension.h"
#import "PBMWebView+PBMTestExtension.h"
#import "PBMOpenMeasurementEventTracker+PBMTestExtension.h"
#import "PBMVideoCreative+PBMTestExtension.h"
#import "PBMVideoView+PBMTestExtension.h"
#import "PBMModalAnimator+PBMTestExtension.h"
#import "PBMBasicParameterBuilder+PBMTestExtension.h"
#import "PBMJSLibraryManager+PBMTestExtension.h"
#import "PBMMRAIDController+PBMTestExtension.h"
#import "PBMCreativeViewabilityTracker+PBMTestExtension.h"

// Prebid
#import "PBMAdLoadFlowController+PrivateState.h"
#import "PBMBaseAdUnit+Protected.h"
#import "PBMBidResponseTransformer.h"
#import "PBMError.h"
#import "PBMPrebidParameterBuilder.h"
#import "PBMWinNotifier+Private.h"

// PBMORTB
#import "PBMORTBPrebid.h"
#import "PBMORTBAbstractResponse+Protected.h"
#import "PBMORTBBidResponse+Internal.h"
#import "PBMORTBSeatBid+Internal.h"

#import "UIView+PBMViewExposure.h"

#import "MediationInterstitialAdUnit+TestExtension.h"
#import "MediationBannerAdUnit+TestExtension.h"
#import "PBMLocationManagerProtocol.h"
