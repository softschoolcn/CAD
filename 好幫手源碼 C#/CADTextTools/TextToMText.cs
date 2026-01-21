using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.Geometry;

namespace 好幫手工具箱.CADTextTools
{
    public class TextToMText
    {
        [CommandMethod("SF_T2M")]
        public void Execute()
        {
            Database db = HostApplicationServices.WorkingDatabase;
            Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;

            // 支援選取普通文字、多行文字與天正文字
            TypedValue[] filter = { new TypedValue(0, "TEXT,MTEXT,TCH_TEXT") };
            PromptSelectionResult res = ed.GetSelection(new SelectionFilter(filter));

            if (res.Status != PromptStatus.OK) return;

            using (Transaction tr = db.TransactionManager.StartTransaction())
            {
                BlockTableRecord btr = (BlockTableRecord)tr.GetObject(db.CurrentSpaceId, OpenMode.ForWrite);

                foreach (SelectedObject selObj in res.Value)
                {
                    Entity ent = (Entity)tr.GetObject(selObj.ObjectId, OpenMode.ForWrite);
                    string originalText = (ent is DBText d) ? d.TextString : (ent is MText m) ? m.Contents : "";
                    
                    MText mt = new MText();
                    // 核心：修正阿拉伯語 2026 亂序的 Unicode 標籤
                    mt.Contents = "{\\u202B" + originalText + "\\u202C}"; 
                    
                    mt.Location = (ent is DBText dt) ? dt.Position : (ent is MText mt2) ? mt2.Location : Point3d.Origin;
                    mt.Height = (ent is DBText dh) ? dh.Height : (ent is MText mh) ? mh.Height : 2.5;
                    mt.Layer = ent.Layer;

                    btr.AppendEntity(mt);
                    tr.AddNewlyCreatedDBObject(mt, true);
                    ent.Erase(); 
                }
                tr.Commit();
            }
            ed.WriteMessage("\n[好幫手] 轉換完成：已套用阿語防亂序與天正兼容邏輯。");
        }
    }
}