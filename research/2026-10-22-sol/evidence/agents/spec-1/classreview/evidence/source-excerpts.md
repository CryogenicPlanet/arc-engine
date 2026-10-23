
d0c02bfad08ec92a6ffcf989e53cd995e6381a4c components/history/core/browser/history_types.h
792: class DeletionInfo {
793:  public:
794:   // Captures the reason for the history deletion.
795:   enum class Reason {
796:     // All foreign visits are being deleted (i.e. visits that occurred on
797:     // another device but were synced to this device).
798:     kDeleteAllForeignVisits,
799:     kOther,
800:   };
801: 
802:   // Returns a DeletionInfo that covers all history.
803:   static DeletionInfo ForAllHistory();
804:   // Returns a DeletionInfo with invalid time range for the given urls.
805:   static DeletionInfo ForUrls(URLRows deleted_rows,
806:                               std::set<GURL> favicon_urls);
807: 
808:   DeletionInfo(const DeletionTimeRange& time_range,
1029: struct AnnotatedVisit {
1030:   AnnotatedVisit();
1031:   AnnotatedVisit(URLRow url_row,
1032:                  VisitRow visit_row,
1033:                  VisitContextAnnotations context_annotations,
1034:                  VisitContentAnnotations content_annotations,
1035:                  VisitID referring_visit_of_redirect_chain_start,
1036:                  VisitID opener_visit_of_redirect_chain_start,
1037:                  VisitSource visit);
1038:   AnnotatedVisit(const AnnotatedVisit&);
1039:   AnnotatedVisit(AnnotatedVisit&&);
1040:   AnnotatedVisit& operator=(const AnnotatedVisit&);
1041:   AnnotatedVisit& operator=(AnnotatedVisit&&);
1042:   ~AnnotatedVisit();
1043: 
1044:   URLRow url_row;
1045:   VisitRow visit_row;

d0c02bfad08ec92a6ffcf989e53cd995e6381a4c components/omnibox/browser/autocomplete_controller.cc
631: void AutocompleteController::Start(const AutocompleteInput& input) {
632:   TRACE_EVENT1("omnibox", "AutocompleteController::Start", "text",
633:                base::UTF16ToUTF8(input.text()));
634:   // Autocomplete is a critical user journey for Omnibox interaction, we exclude
635:   // it from additional memory safety checks.
636:   // TODO(crbug.com/478634529): Optimize and remove if possible.
637:   base::ScopedSafetyChecksExclusion excluded;
638: 
639:   // Providers assume synchronous inputs (`omit_asynchronous_matches() ==
640:   // true`) are not zero-suggest ones. See crbug.com/1339425.
641:   DCHECK(!input.omit_asynchronous_matches() || !input.IsZeroSuggest());
797: void AutocompleteController::Stop(AutocompleteStopReason stop_reason) {
798:   // Autocomplete is a critical user journey for Omnibox interaction, we exclude
799:   // it from additional memory safety checks.
800:   // TODO(crbug.com/478634529): Optimize and remove if possible.
801:   base::ScopedSafetyChecksExclusion excluded;
802:   // Must be called before `expire_timer_.Stop()`, modifying `done_`, or
803:   // modifying `AutocompleteProvider::done_` below. If the current request has
804:   // not completed, and therefore has not been logged yet, will log it now.
805:   // Likewise, if the providers have not completed, and therefore have not been
806:   // logged yet, will log them now.
807:   metrics_.OnStop();
808: 

d0c02bfad08ec92a6ffcf989e53cd995e6381a4c components/tab_groups/tab_group_visual_data.h
16: namespace tab_groups {
17: 
18: class COMPONENT_EXPORT(TAB_GROUPS) TabGroupVisualData {
19:  public:
20:   // Construct a TabGroupVisualData with placeholder name and random color.
21:   TabGroupVisualData();
22:   TabGroupVisualData(std::u16string title,
23:                      tab_groups::TabGroupColorId color,
24:                      bool is_collapsed = false);
25:   TabGroupVisualData(std::u16string title,
26:                      uint32_t color_int,
27:                      bool is_collapsed = false);
28: 
29:   TabGroupVisualData(const TabGroupVisualData& other) = default;
30:   TabGroupVisualData(TabGroupVisualData&& other) = default;
31: 
32:   TabGroupVisualData& operator=(const TabGroupVisualData& other) = default;
33:   TabGroupVisualData& operator=(TabGroupVisualData&& other) = default;
34: 
35:   const std::u16string& title() const { return title_; }
36:   const tab_groups::TabGroupColorId& color() const { return color_; }
37:   bool is_collapsed() const { return is_collapsed_; }
38: 
39:   // Checks whether two instances are visually equivalent.
40:   auto operator<=>(const TabGroupVisualData&) const = default;
41: 
42:   void SetTitle(const std::u16string& title) { title_ = title; }
43: 
44:  private:
45:   std::u16string title_;

d0c02bfad08ec92a6ffcf989e53cd995e6381a4c content/browser/media/session/media_session_impl.cc
704: void MediaSessionImpl::Resume(SuspendType suspend_type) {
705:   // If the site has registered an action handler for play, we should pass it to
706:   // the site and let them handle it.
707:   if (suspend_type == SuspendType::kUI &&
708:       ShouldRouteAction(media_session::mojom::MediaSessionAction::kPlay)) {
709:     DidReceiveAction(media_session::mojom::MediaSessionAction::kPlay);
710:     return;
711:   }
712: 
713:   // When the resume requests comes from another source than system, audio focus
739: void MediaSessionImpl::Suspend(SuspendType suspend_type) {
740:   if (!IsActive())
741:     return;
742: 
743:   if (suspend_type == SuspendType::kUI) {
744:     // If the site has registered an action handler for pause then we should
745:     // pass it to the site and let them handle it.
746:     if (ShouldRouteAction(media_session::mojom::MediaSessionAction::kPause)) {
747:       DidReceiveAction(media_session::mojom::MediaSessionAction::kPause);
748:       return;
1296: void MediaSessionImpl::SeekTo(base::TimeDelta seek_time) {
1297:   // If the site has registered an action handler for seek to then we
1298:   // should pass it to the site and let them handle it.
1299:   if (ShouldRouteAction(media_session::mojom::MediaSessionAction::kSeekTo)) {
1300:     DidReceiveAction(media_session::mojom::MediaSessionAction::kSeekTo,
1301:                      blink::mojom::MediaSessionActionDetails::NewSeekTo(
1302:                          blink::mojom::MediaSessionSeekToDetails::New(
1303:                              seek_time, /*fast_seek=*/false)));
1304:     return;
1305:   }
1306: 
1307:   for (const auto& it : normal_players_)
1308:     it.first.observer->OnSeekTo(it.first.player_id, seek_time);
1412: void MediaSessionImpl::SetMute(bool mute) {
1413:   // The SetMute action should only be available when there is one normal
1414:   // player, though due to the asynchronous nature of mojo, we may no longer
1415:   // have 1 normal player. In that case, just return.
1416:   if (normal_players_.size() != 1u) {
1417:     return;
1418:   }
1419:   normal_players_.begin()->first.observer->OnSetMute(
1420:       normal_players_.begin()->first.player_id, mute);
1421: }
1422: 
1423: void MediaSessionImpl::RequestMediaRemoting() {
1424:   // The RequestMediaRemoting action should only be available when there is one

d0c02bfad08ec92a6ffcf989e53cd995e6381a4c chrome/browser/ui/thumbnails/thumbnail_tab_helper.cc
242: void ThumbnailTabHelper::CaptureThumbnailOnTabBackgrounded() {
243:   if (!state_->is_ready()) {
244:     return;
245:   }
246: 
247:   const base::TimeTicks time_of_call = base::TimeTicks::Now();
248: 
249:   // Ignore previous requests to capture a thumbnail on tab switch.
250:   weak_factory_for_thumbnail_on_tab_hidden_.InvalidateWeakPtrs();
251: 
252:   // Get the WebContents' main view. Note that during shutdown there may not be
253:   // a view to capture, and views are sometimes not available for capture even
254:   // when they are present.
255:   content::RenderWidgetHostView* const source_view = state_->GetView();
256:   if (!source_view || !source_view->IsSurfaceAvailableForCopy()) {
257:     return;
258:   }
356: void ThumbnailTabHelper::StartVideoCapture() {
357:   content::RenderWidgetHostView* const source_view = state_->GetView();
358:   if (!source_view) {
359:     return;
360:   }
361: 
362:   const float scale_factor = source_view->GetDeviceScaleFactor();
363:   const gfx::Size source_size = source_view->GetViewBounds().size();
364:   if (source_size.IsEmpty()) {
365:     return;
366:   }
367: 
368:   start_video_capture_time_ = base::TimeTicks::Now();
369: 

d0c02bfad08ec92a6ffcf989e53cd995e6381a4c components/site_engagement/content/site_engagement_score.h
29: class SiteEngagementScore {
30:  public:
31:   // The parameters which can be varied via field trial.
32:   enum Variation {
33:     // The maximum number of points that can be accrued in one day.
34:     MAX_POINTS_PER_DAY = 0,
35: 
36:     // The period over which site engagement decays.
37:     DECAY_PERIOD_IN_HOURS,
38: 
39:     // The number of points to decay per period.
148:   double GetTotalScore() const;
149: 
150:   // Returns a structure containing the origin URL and score, and details
151:   // of the base and bonus scores. Note that the |score| is limited to
152:   // kMaxPoints, while the detailed scores are returned raw.
153:   mojom::SiteEngagementDetails GetDetails() const;
154: 
155:   // Writes the values in this score into |settings_map_|.
156:   void Commit();
157: 

d0c02bfad08ec92a6ffcf989e53cd995e6381a4c components/net_log/net_export_file_writer.cc
275:   DCHECK(thread_checker_.CalledOnValidThread());
276: 
277:   base::DictValue dict;
278:   dict.Set("file", base::UTF16ToUTF8(log_path_.LossyDisplayName()));
279: 
280:   std::string_view state_string;
281:   switch (state_) {
282:     case STATE_UNINITIALIZED:
283:       state_string = "UNINITIALIZED";
284:       break;
285:     case STATE_INITIALIZING:
286:       state_string = "INITIALIZING";
287:       break;
288:     case STATE_NOT_LOGGING:
289:       state_string = "NOT_LOGGING";
290:       break;
291:     case STATE_STARTING_LOG:
292:       state_string = "STARTING_LOG";
293:       break;
294:     case STATE_LOGGING:
295:       state_string = "LOGGING";
296:       break;
297:     case STATE_STOPPING_LOG:
298:       state_string = "STOPPING_LOG";
299:       break;
300:   }
301:   dict.Set("state", state_string);
302: 
303:   dict.Set("logExists", log_exists_);
304:   dict.Set("logCaptureModeKnown", log_capture_mode_known_);
305:   dict.Set("captureMode", CaptureModeToString(log_capture_mode_));
306:   dict.Set("logFileFormatKnown", log_file_format_known_);
307:   dict.Set("fileFormat", FileFormatToString(log_file_format_));
308: 
309:   return dict;
310: }
311: 
312: bool NetExportFileWriter::IsLogging() const {
313:   DCHECK(thread_checker_.CalledOnValidThread());
314:   return state_ == STATE_LOGGING || state_ == STATE_STARTING_LOG ||

d0c02bfad08ec92a6ffcf989e53cd995e6381a4c chrome/browser/ui/thumbnails/thumbnail_tab_helper.h
27: // Maintains the thumbnail image shown in e.g. tab hover cards. Owned by the
28: // tab's TabFeatures; only created when a feature that needs thumbnails is
29: // enabled.
30: class ThumbnailTabHelper : public content::WebContentsObserver {
31:  public:
32:   DECLARE_USER_DATA(ThumbnailTabHelper);
33: 
34:   // `contents` is passed explicitly because during a discard the helper is
35:   // recreated for the incoming WebContents before `tab` swaps its contents.
36:   ThumbnailTabHelper(tabs::TabInterface& tab, content::WebContents* contents);
37: 
38:   ThumbnailTabHelper(const ThumbnailTabHelper&) = delete;
39:   ThumbnailTabHelper& operator=(const ThumbnailTabHelper&) = delete;
40: 
41:   ~ThumbnailTabHelper() override;
42: 
43:   static ThumbnailTabHelper* From(tabs::TabInterface* tab);
44: 
45:   scoped_refptr<ThumbnailImage> thumbnail() const { return thumbnail_; }
46: 
47:   bool is_tab_discarded() const { return is_tab_discarded_; }
48: 
49:   // Notify the helper that the tab is being hidden by being put into the
50:   // background. Allows for an updated preview image after swapping away from an
51:   // active tab.
52:   void CaptureThumbnailOnTabBackgrounded();

426920c33ef22551e0b8f48cf343d623ac25642d cef_browser.h
633:   virtual bool SendDevToolsMessage(const void* message,
634:                                    size_t message_size) = 0;
635: 
636:   ///
637:   /// Execute a method call over the DevTools protocol. This is a more
638:   /// structured version of SendDevToolsMessage. |message_id| is an incremental
639:   /// number that uniquely identifies the message (pass 0 to have the next
640:   /// number assigned automatically based on previous values). |method| is the
641:   /// method name. |params| are the method parameters, which may be empty. See
642:   /// the DevTools protocol documentation (linked above) for details of
643:   /// supported methods and the expected |params| dictionary contents. This
644:   /// method will return the assigned message ID if called on the UI thread and
645:   /// the message was successfully submitted for validation, otherwise 0. See
646:   /// the SendDevToolsMessage documentation for additional usage information.
647:   ///
648:   /*--cef(optional_param=params)--*/
649:   virtual int ExecuteDevToolsMethod(int message_id,
650:                                     const CefString& method,
651:                                     CefRefPtr<CefDictionaryValue> params) = 0;
652: 
653:   ///
654:   /// Add an observer for DevTools protocol messages (method results and
655:   /// events). The observer will remain registered until the returned
656:   /// Registration object is destroyed. See the SendDevToolsMessage
657:   /// documentation for additional usage information.
658:   ///
659:   /*--cef()--*/
660:   virtual CefRefPtr<CefRegistration> AddDevToolsMessageObserver(
661:       CefRefPtr<CefDevToolsMessageObserver> observer) = 0;
662: 
663:   ///
995:   virtual void SetAutoResizeEnabled(bool enabled,
996:                                     const CefSize& min_size,
997:                                     const CefSize& max_size) = 0;
998: 
999:   ///
1000:   /// Set whether the browser's audio is muted.
1001:   ///
1002:   /*--cef()--*/
1003:   virtual void SetAudioMuted(bool mute) = 0;
1004: 
1005:   ///
1006:   /// Returns true if the browser's audio is muted.  This method can only be
1007:   /// called on the UI thread.

426920c33ef22551e0b8f48cf343d623ac25642d cef_types_mac.h
148:   cef_window_handle_t view;
149: 
150:   ///
151:   /// Optionally change the runtime style. Alloy style will always be used if
152:   /// |windowless_rendering_enabled| is true or if |parent_view| is provided.
153:   /// See cef_runtime_style_t documentation for details.
154:   ///
155:   cef_runtime_style_t runtime_style;
156: } cef_window_info_t;
157: 
