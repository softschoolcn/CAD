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

            // 支援選取普通文字與天正文字
            TypedValue[] filter = { new TypedValue(0, "TEXT,MTEXT,TCH_TEXT") };
            PromptSelectionResult res = ed.GetSelection(new SelectionFilter(filter));

            if (res.Status != PromptStatus.OK) return;

            using (Transaction tr = db.TransactionManager.StartTransaction())
            {
                BlockTableRecord btr = (BlockTableRecord)tr.GetObject(db.CurrentSpaceId, OpenMode.ForWrite);

                foreach (SelectedObject selObj in res.Value)
                {
                    Entity ent = (Entity)tr.GetObject(selObj.ObjectId, OpenMode.ForWrite);
                    string originalText = GetTextContent(ent);
                    
                    MText mt = new MText();
                    // 這裡就是修正阿拉伯語 2026 亂序的關鍵
                    mt.Contents = "{\\u202B" + originalText + "\\u202C}"; 
                    mt.Location = GetPosition(ent);
                    mt.Height = GetHeight(ent);
                    mt.Layer = ent.Layer;

                    btr.AppendEntity(mt);
                    tr.AddNewlyCreatedDBObject(mt, true);
                    ent.Erase(); 
                }
                tr.Commit();
            }
        }

        private string GetTextContent(Entity ent) => (ent is DBText d) ? d.TextString : (ent is MText m) ? m.Contents : "";
        private Point3d GetPosition(Entity ent) => (ent is DBText d) ? d.Position : (ent is MText m) ? m.Location : Point3d.Origin;
        private double GetHeight(Entity ent) => (ent is DBText d) ? d.Height : (ent is MText m) ? m.Height : 2.5;
    }
}