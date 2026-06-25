import streamlit as st
import sqlite3
import pandas as pd
import os
import subprocess

st.set_page_config(page_title="WAL Control Panel", layout="wide")

st.title("WAL Storage Engine Dashboard")

db_path = "metadata.db"
wal_path = "wal.log"

col1, col2, col3 = st.columns(3)

if os.path.exists(db_path):
    conn = sqlite3.connect(db_path)
    df = pd.read_sql_query("SELECT * FROM log_index ORDER BY transaction_id ASC", conn)
    conn.close()

    total_volume = df["volume"].sum() if not df.empty else 0.0
    total_tx = len(df)
    
    col1.metric("Total Transactions", total_tx)
    col2.metric("Total Volume Processed", f"{total_volume:,.2f}")
    
    st.subheader("Indexed Records")
    st.dataframe(df, use_container_width=True)
else:
    col1.metric("Total Transactions", 0)
    col2.metric("Total Volume Processed", 0.0)
    st.info("No metadata found.")

wal_size = os.path.getsize(wal_path) if os.path.exists(wal_path) else 0
col3.metric("wal.log Size (Bytes)", wal_size)

st.markdown("---")

if st.button("Simulate System Crash", type="primary"):
    exe_name = "engine.exe" if os.name == "nt" else "./engine"
    try:
        result = subprocess.run([exe_name, "--recover"], capture_output=True, text=True, check=True)
        st.success("Crash simulation and recovery executed.")
        st.text("Recovery Console Output:")
        st.code(result.stdout)
    except FileNotFoundError:
        st.error("Engine executable not found.")
    except subprocess.CalledProcessError as e:
        st.error(f"Process failed with error:\n{e.stderr}")
